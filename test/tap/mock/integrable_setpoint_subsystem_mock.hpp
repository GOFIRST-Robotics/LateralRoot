/*
 * Copyright (c) 2022 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_INTEGRABLE_SETPOINT_SUBSYSTEM_MOCK_HPP_
#define TAPROOT_INTEGRABLE_SETPOINT_SUBSYSTEM_MOCK_HPP_

#include <gmock/gmock.h>

#include "tap/control/setpoint/interfaces/integrable_setpoint_subsystem.hpp"

namespace tap::mock
{
/**
 * A class for mocking a setpoint subsystem. Will by default act as if it
 * were unjammed and online (specified in mock_constructors.cpp)
 */
class IntegrableSetpointSubsystemMock : public control::setpoint::IntegrableSetpointSubsystem
{
public:
    IntegrableSetpointSubsystemMock(tap::Drivers* drivers);
    virtual ~IntegrableSetpointSubsystemMock();

    MOCK_METHOD(float, getSetpoint, (), (const override));
    MOCK_METHOD(void, setSetpoint, (float), (override));
    MOCK_METHOD(float, getCurrentValue, (), (const override));
    MOCK_METHOD(float, getJamSetpointTolerance, (), (const override));
    MOCK_METHOD(bool, calibrateHere, (), (override));
    MOCK_METHOD(bool, isJammed, (), (override));
    MOCK_METHOD(void, clearJam, (), (override));
    MOCK_METHOD(bool, isCalibrated, (), (override));
    MOCK_METHOD(bool, isOnline, (), (override));
    MOCK_METHOD(float, getVelocity, (), (override));
    MOCK_METHOD(float, getCurrentValueIntegral, (), (const override));
    MOCK_METHOD(void, refreshSafeDisconnect, (), (override));
};

}  // namespace tap::mock

#endif  // TAPROOT_INTEGRABLE_SETPOINT_SUBSYSTEM_MOCK_HPP_