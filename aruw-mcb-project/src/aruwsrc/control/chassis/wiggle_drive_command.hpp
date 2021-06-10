/*
 * Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
 *
 * This file is part of aruw-mcb.
 *
 * aruw-mcb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aruw-mcb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aruw-mcb.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WIGGLE_DRIVE_COMMAND_HPP_
#define WIGGLE_DRIVE_COMMAND_HPP_

#include <aruwlib/control/command.hpp>

#include "aruwlib/algorithms/ramp.hpp"

#include "aruwsrc/control/turret/turret_subsystem.hpp"

namespace aruwlib
{
class Drivers;
}

namespace aruwsrc
{
namespace chassis
{
class ChassisSubsystem;

/**
 * A command that automatically rotates the chassis back and forth, following
 * a sine wave centered around the yaw gimbal angle, while still allowing for
 * translational movement.
 */
class WiggleDriveCommand : public aruwlib::control::Command
{
public:
    WiggleDriveCommand(
        aruwlib::Drivers* drivers,
        ChassisSubsystem* chassis,
        aruwsrc::turret::TurretSubsystem* turret);

    void initialize() override;

    /**
     * Updates the sine wave used for wiggling, updates the rotation PD controller,
     * and applies a rotation matrix to the <x, y> vector before passing these to
     * the chassis subsystem's `setDesiredOutput` function.
     */
    void execute() override;

    void end(bool) override;

    bool isFinished() const override;

    const char* getName() const override { return "chassis wiggle drive"; }

private:
    static constexpr float WIGGLE_PERIOD = 2000.0f;
    static constexpr float WIGGLE_MAX_ROTATE_ANGLE = 45.0f;
    static constexpr float WIGGLE_ROTATE_KP = -250.0f;
    static constexpr float TRANSLATIONAL_SPEED_FRACTION_WHILE_WIGGLING = 0.5f;
    static constexpr float WIGGLE_OUT_OF_CENTER_MAX_ROTATE_ERR = 10.0f;
    static constexpr float TURRET_YAW_TARGET_RAMP_INCREMENT = 0.5f;

    aruwlib::Drivers* drivers;
    ChassisSubsystem* chassis;
    aruwsrc::turret::TurretSubsystem* turret;

    uint32_t timeOffset = 0;
    float startTimeForAngleOffset = 0.0f;
    bool outOfCenter = false;
    aruwlib::algorithms::Ramp turretYawRamp;

    // sin curve to determine angle to rotate to based on current "time"
    float wiggleSin(float time);
};  // class WiggleDriveCommand

}  // namespace chassis

}  // namespace aruwsrc

#endif  // WIGGLE_DRIVE_COMMAND_HPP_
