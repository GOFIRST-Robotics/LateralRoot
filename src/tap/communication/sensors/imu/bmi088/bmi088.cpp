/*
 * Copyright (c) 2020-2022 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of Taproot.
 *
 * Taproot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Taproot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Taproot.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "bmi088.hpp"

#include "tap/architecture/endianness_wrappers.hpp"
#include "tap/board/board.hpp"
#include "tap/drivers.hpp"
#include "tap/errors/create_errors.hpp"

#include "modm/math/geometry/angle.hpp"
#include "modm/math/units.hpp"

#include "bmi088_data.hpp"
#include "bmi088_hal.hpp"

using namespace modm::literals;
using namespace tap::arch;
using namespace Board;

namespace tap::communication::sensors::imu::bmi088
{
#if defined(PLATFORM_HOSTED)
#define DELAY_MS(ms)
#define DELAY_US(us)
#else
#define DELAY_MS(ms) modm::delay_ms(ms);
#define DELAY_US(us) modm::delay_us(us);
#endif

void Bmi088::initialize(float sampleFrequency, float mahonyKp, float mahonyKi)
{
    AbstractIMU::initialize(sampleFrequency, mahonyKp, mahonyKi);
#if !defined(PLATFORM_HOSTED)
    ImuCS1Accel::GpioOutput();
    ImuCS1Gyro::GpioOutput();

    DELAY_MS(100);

    ImuSpiMaster::connect<ImuMiso::Miso, ImuMosi::Mosi, ImuSck::Sck>();
    ImuSpiMaster::initialize<SystemClock, 10_MHz>();

    DELAY_MS(1);
#endif

    imuState = ImuState::IMU_NOT_CALIBRATED;

    initializeAcc();
    initializeGyro();

    imuHeater.initialize();
}

void Bmi088::initializeAcc()
{
    // Write to the accelerometer a few times to get it to wake up (without this the bmi088 will not
    // turn on properly from cold boot).
    Bmi088Hal::bmi088AccReadSingleReg(Acc::ACC_CHIP_ID);
    DELAY_MS(1);
    Bmi088Hal::bmi088AccReadSingleReg(Acc::ACC_CHIP_ID);
    DELAY_MS(1);

    // Page 13 of the bmi088 datasheet states:
    // After the POR (power-on reset) the gyroscope is in normal mode, while the accelerometer is in
    // suspend mode. To switch the accelerometer into normal mode, the user must perform the
    // following steps:
    // a. Power up the sensor
    // b. Wait 1 ms
    // c. Enter normal mode by writing '4' to ACC_PWR_CTRL
    // d. Wait for 450 microseconds

    Bmi088Hal::bmi088AccWriteSingleReg(Acc::ACC_PWR_CTRL, Acc::AccPwrCtrl::ACCELEROMETER_ON);

    DELAY_US(450);

    // read ACC_CHIP_ID to start SPI communication
    // Page 45 of the bmi088 datasheet states:
    // "To change the sensor to SPI mode in the initialization phase, the user
    // could perform a dummy SPI read operation"
    Bmi088Hal::bmi088AccReadSingleReg(Acc::ACC_CHIP_ID);

    // check communication is normal after reset
    uint8_t readChipID = Bmi088Hal::bmi088AccReadSingleReg(Acc::ACC_CHIP_ID);
    DELAY_MS(1);

    if (readChipID != Acc::ACC_CHIP_ID_VALUE)
    {
        RAISE_ERROR(drivers, "bmi088 accel init failed");
        imuState = ImuState::IMU_NOT_CONNECTED;
        return;
    }

    setAndCheckAccRegister(
        Acc::ACC_CONF,
        Acc::AccBandwidth_t(accOversampling) | Acc::AccOutputRate_t(accOutputRate));

    setAndCheckAccRegister(Acc::ACC_RANGE, ACC_RANGE);
}

void Bmi088::initializeGyro()
{
    // reset gyro
    Bmi088Hal::bmi088GyroWriteSingleReg(Gyro::GYRO_SOFTRESET, Gyro::GyroSoftreset::RESET_SENSOR);
    DELAY_MS(80);

    // check communication normal after reset
    Bmi088Hal::bmi088GyroReadSingleReg(Gyro::GYRO_CHIP_ID);
    DELAY_MS(1);
    uint8_t res = Bmi088Hal::bmi088GyroReadSingleReg(Gyro::GYRO_CHIP_ID);
    DELAY_MS(1);

    if (res != Gyro::GYRO_CHIP_ID_VALUE)
    {
        RAISE_ERROR(drivers, "bmi088 gyro init failed");
        imuState = ImuState::IMU_NOT_CONNECTED;
    }

    setAndCheckGyroRegister(Gyro::GYRO_RANGE, GYRO_RANGE);

    // extra 0x80 is because the bandwidth register will always have 0x80 masked
    // so when checking, we want to mask as well to avoid an error
    setAndCheckGyroRegister(Gyro::GYRO_BANDWIDTH, gyroOutputRate | Gyro::GyroBandwidth_t(0x80));

    setAndCheckGyroRegister(Gyro::GYRO_LPM1, Gyro::GyroLpm1::PWRMODE_NORMAL);
}

void Bmi088::periodicIMUUpdate()
{
    AbstractIMU::periodicIMUUpdate();
    imuHeater.runTemperatureController(imuData.temperature);
}

bool Bmi088::read()
{
    if (!readTimeout.execute())
    {
        return false;
    }

    uint8_t rxBuff[6] = {};

    Bmi088Hal::bmi088AccReadMultiReg(Acc::ACC_X_LSB, rxBuff, 6);

    prevIMUDataReceivedTime = tap::arch::clock::getTimeMicroseconds();

    float rawAccX = bigEndianInt16ToFloat(rxBuff);
    float rawAccY = bigEndianInt16ToFloat(rxBuff + 2);
    float rawAccZ = bigEndianInt16ToFloat(rxBuff + 4);
    imuData.accRaw = tap::algorithms::transforms::Vector(rawAccX, rawAccY, rawAccZ);

    Bmi088Hal::bmi088GyroReadMultiReg(Gyro::RATE_X_LSB, rxBuff, 6);

    float rawGyroX = bigEndianInt16ToFloat(rxBuff);
    float rawGyroY = bigEndianInt16ToFloat(rxBuff + 2);
    float rawGyroZ = bigEndianInt16ToFloat(rxBuff + 4);
    imuData.gyroRaw = tap::algorithms::transforms::Vector(rawGyroX, rawGyroY, rawGyroZ);

    Bmi088Hal::bmi088AccReadMultiReg(Acc::TEMP_MSB, rxBuff, 2);
    imuData.temperature = parseTemp(rxBuff[0], rxBuff[1]);

    imuData.gyroRadPerSec =
        (imuData.gyroRaw - imuData.gyroOffsetRaw) * GYRO_RAD_PER_S_PER_GYRO_COUNT;
    imuData.accG = (imuData.accRaw - imuData.accOffsetRaw) * ACC_G_PER_ACC_COUNT;

    applyTransform(imuData);

    return true;
}

void Bmi088::setAndCheckAccRegister(Acc::Register reg, Acc::Registers_t value)
{
    Bmi088Hal::bmi088AccWriteSingleReg(reg, value);
    DELAY_US(150);

    uint8_t val = Bmi088Hal::bmi088AccReadSingleReg(reg);
    DELAY_US(150);

    if (val != value.value)
    {
        RAISE_ERROR(drivers, "bmi088 acc config failed");
        imuState = ImuState::IMU_NOT_CONNECTED;
    }
}

void Bmi088::setAndCheckGyroRegister(Gyro::Register reg, Gyro::Registers_t value)
{
    Bmi088Hal::bmi088GyroWriteSingleReg(reg, value);
    DELAY_US(150);

    uint8_t val = Bmi088Hal::bmi088GyroReadSingleReg(reg);
    DELAY_US(150);

    if (val != value.value)
    {
        RAISE_ERROR(drivers, "bmi088 gyro config failed");
        imuState = ImuState::IMU_NOT_CONNECTED;
    }
}

}  // namespace tap::communication::sensors::imu::bmi088
