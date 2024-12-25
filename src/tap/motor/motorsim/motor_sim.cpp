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

#ifdef PLATFORM_HOSTED

#include "motor_sim.hpp"

#include "tap/algorithms/math_user_utils.hpp"
#include "tap/architecture/clock.hpp"

using namespace tap::algorithms;

namespace tap::motor::motorsim
{
MotorSim::MotorSim(const Config &config) : config(config) { reset(); }

void MotorSim::reset()
{
    enc = 0;
    rpm = 0;
    input = 0;
}

void MotorSim::setMotorInput(int16_t in)
{
    input = limitVal<int16_t>(in, -config.maxInputMag, config.maxInputMag);
}

void MotorSim::setLoad(float load) { this->load = limitVal(load, -getMaxTorque(), getMaxTorque()); }

void MotorSim::update()
{
    static constexpr float MILLISECOND_PER_MINUTE = 60'000.0f;

    uint32_t curTime = tap::arch::clock::getTimeMilliseconds();
    float dt = curTime - prevTime;
    prevTime = curTime;

    rpm = (config.maxW - config.wtGrad * load) * getCurrent() / config.currentLim;

    enc = static_cast<int>(enc + config.maxencoder * rpm * dt / MILLISECOND_PER_MINUTE) %
          config.maxencoder;
}

float MotorSim::getCurrent() const { return config.maxCurrent * input / config.maxInputMag; }

int16_t MotorSim::getEnc() const { return enc; }

int16_t MotorSim::getInput() const { return input; }

float MotorSim::getMaxTorque() const { return config.maxCurrent * config.kt; }

int16_t MotorSim::getRPM() const { return rpm; }

}  // namespace tap::motor::motorsim

#endif  // PLATFORM_HOSTED