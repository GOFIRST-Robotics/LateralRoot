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

#include "calibrate_command.hpp"

#include "tap/control/setpoint/interfaces/setpoint_subsystem.hpp"
#include "tap/control/subsystem.hpp"

using tap::control::setpoint::SetpointSubsystem;

namespace tap::control::setpoint
{
CalibrateCommand::CalibrateCommand(SetpointSubsystem* setpointSubsystem)
    : setpointSubsystem(setpointSubsystem)
{
    this->addSubsystemRequirement(setpointSubsystem);
}

bool CalibrateCommand::isReady() { return setpointSubsystem->isOnline(); }

// No initialization necessary
void CalibrateCommand::initialize() { calibrationSuccessful = false; }

void CalibrateCommand::execute() { calibrationSuccessful = setpointSubsystem->calibrateHere(); }

void CalibrateCommand::end(bool) {}

bool CalibrateCommand::isFinished() const { return calibrationSuccessful; }

}  // namespace tap::control::setpoint
