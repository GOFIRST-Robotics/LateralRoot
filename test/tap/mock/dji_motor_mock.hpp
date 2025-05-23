/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_DJI_MOTOR_MOCK_HPP_
#define TAPROOT_DJI_MOTOR_MOCK_HPP_

#include <gmock/gmock.h>

#include "tap/drivers.hpp"
#include "tap/motor/dji_motor.hpp"

#include "modm/architecture/interface/can_message.hpp"

#include "dji_motor_encoder_mock.hpp"

namespace tap
{
namespace mock
{
class DjiMotorMock : public tap::motor::DjiMotor
{
public:
    DjiMotorMock(
        Drivers* drivers,
        tap::motor::MotorId desMotorIdentifier,
        tap::can::CanBus motorCanBus,
        bool isInverted,
        const char* name,
        bool currentControl = false,
        float gearRatio = 1,
        uint32_t encoderHomePosition = 0,
        tap::encoder::EncoderInterface* externalEncoder = nullptr);
    virtual ~DjiMotorMock();

    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(void, processMessage, (const modm::can::Message& message), (override));
    MOCK_METHOD(void, setDesiredOutput, (int32_t desiredOutput), (override));
    MOCK_METHOD(void, resetEncoderValue, (), (override));
    MOCK_METHOD(bool, isMotorOnline, (), (const override));
    MOCK_METHOD(bool, isInCurrentControl, (), (const override));
    MOCK_METHOD(void, serializeCanSendData, (modm::can::Message * txMessage), (const override));

    MOCK_METHOD(int16_t, getOutputDesired, (), (const override));
    MOCK_METHOD(uint32_t, getMotorIdentifier, (), (const override));
    MOCK_METHOD(int8_t, getTemperature, (), (const override));
    MOCK_METHOD(int16_t, getTorque, (), (const override));
    MOCK_METHOD(bool, isMotorInverted, (), (const override));
    MOCK_METHOD(tap::can::CanBus, getCanBus, (), (const override));
    MOCK_METHOD(const char*, getName, (), (const override));

};  // class DjiMotor

}  // namespace mock

}  // namespace tap

#endif  // TAPROOT_DJI_MOTOR_MOCK_HPP_
