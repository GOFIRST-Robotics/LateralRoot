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

#ifndef TAPROOT_UNJAM_INTEGRAL_COMMAND_HPP_
#define TAPROOT_UNJAM_INTEGRAL_COMMAND_HPP_

#include <cstdint>

#include "tap/architecture/timeout.hpp"
#include "tap/control/command.hpp"

#include "../interfaces/integrable_setpoint_subsystem.hpp"

#include "unjam_command_interface.hpp"

namespace tap::control::setpoint
{
/**
 * Command that takes control of a velocity setpoint subsystem moves it back and forth.
 * One back and forward motion counts as a cycle. Unjamming cycles start by trying
 * to move in negative direction before trying to move in positive direction.
 *
 * If the unjam command successfully clears its forward and backward threshold it will
 * will clear the velocity setpoint subsystem's jam and end. If not successful after some number of
 * cycle counts, the command will give up and end without clearing the jam.
 *
 * Unlike the unjam command, this command will displace the integral by some amount in
 * order to facilitate unjamming.
 *
 * Like most velocity commands this one will not schedule/will deschedule if
 * IntegrableSetpointSubsystem goes offline.
 */
class UnjamIntegralCommand : public UnjamCommandInterface
{
public:
    /// Config struct that the user passes into the UnjamIntegralCommand's constructor.
    struct Config
    {
        /**
         * The target integral setpoint from the current integral value with units `units * seconds`
         * (units of setpoint integrated with respect to time) that the integral setpoint subsystem
         * will move back and forth by with unjamming.
         *
         * @attention This value must be positive and > 0.
         */
        float targetUnjamIntegralChange;
        /**
         * The target setpoint in units that the integral setpoint subsystem will move
         * back and forth at.
         *
         * @attention This value must be positive and > 0.
         */
        float unjamSetpoint;
        /**
         * The maximum amount of time the controller will wait for the subsystem to reach
         * targetUnjamIntegralChange in milliseconds before trying to move in the opposite
         * direction.
         *
         * @attention This value must be > 1000 * (targetUnjamIntegralChange / unjamSetpoint) since
         * this is the minimum possible time it will take for the motor to rotate
         * targetUnjamIntegralChange units.
         */
        uint32_t maxWaitTime;
        /**
         * The number of cycles to attempt to rotate the velocity setpoint subsystem back and
         * forth.
         *
         * @attention This value must be positive and > 0.
         */
        uint16_t targetCycleCount;
    };

    /**
     * @param[in] integrableSetpointSubsystem The associated agitator subsystem to control.
     */
    UnjamIntegralCommand(
        IntegrableSetpointSubsystem& integrableSetpointSubsystem,
        const Config& config);

    bool isReady() override;

    void initialize() override;

    void execute() override;

    void end(bool interrupted) override;

    bool isFinished() const override;

    const char* getName() const override { return "unjam move integral"; }

private:
    enum UnjamState
    {
        UNJAM_BACKWARD,  ///< The subsystem is being commanded backwards
        UNJAM_FORWARD,   ///< The subsystem is being commanded forwards
        JAM_CLEARED,     ///< The jam is cleared, the subsystem is no longer being told to move.
    };

    IntegrableSetpointSubsystem& integrableSetpointSubsystem;

    Config config;

    /**
     * Timeout for time allowed to rotate past the `unjamThreshold`.
     */
    arch::MilliTimeout unjamRotateTimeout;

    /**
     * counts the number of times the subsystem has been commanded backwards
     */
    uint16_t backwardsCount;

    UnjamState currUnjamState;

    float positionBeforeUnjam;

    bool backwardsCleared;

    bool forwardsCleared;

    void beginUnjamForwards();

    void beginUnjamBackwards();
};

}  // namespace tap::control::setpoint

#endif  // TAPROOT_UNJAM_INTEGRAL_COMMAND_HPP_
