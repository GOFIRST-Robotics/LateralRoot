/*
 * Copyright (c) 2024-2025 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_SEQUENTIAL_COMMAND_HPP_
#define TAPROOT_SEQUENTIAL_COMMAND_HPP_

#include <array>

#include "modm/architecture/interface/assert.hpp"

#include "command.hpp"
#include "command_scheduler_types.hpp"

namespace tap
{
namespace control
{
/**
 * A generic extendable class for implementing a command. Each
 * command is attached to a subsystem. To create a new command,
 * extend the Command class and instantiate the virtual functions
 * in this class. See example_command.hpp for example of this.
 */
template <size_t COMMANDS>
class SequentialCommand : public Command
{
public:
    SequentialCommand(std::array<Command*, COMMANDS> commands)
        : Command(),
          commands(commands),
          currentCommand(0)
    {
        for (Command* command : commands)
        {
            modm_assert(
                command != nullptr,
                "SequentialCommand::SequentialCommand",
                "Null pointer command passed into sequential command.");
            this->commandRequirementsBitwise |= (command->getRequirementsBitwise());
        }
    }

    const char* getName() const override
    {
        return this->currentCommand == COMMANDS ? ""
                                                : this->commands[this->currentCommand]->getName();
    }

    bool isReady() override { return this->commands[0]->isReady(); }

    void initialize() override
    {
        this->currentCommand = 0;
        this->commandInitialized = false;
    }

    void execute() override
    {
        if (!this->commandInitialized)
        {
            if (this->commands[this->currentCommand]->isReady())
            {
                this->commands[this->currentCommand]->initialize();
                this->commandInitialized = true;
            }
        }

        if (this->commandInitialized)
        {
            this->commands[this->currentCommand]->execute();

            if (this->commands[this->currentCommand]->isFinished())
            {
                this->commands[this->currentCommand]->end(false);
                this->commandInitialized = false;
                this->currentCommand++;
            }
        }
    }

    void end(bool interrupted) override
    {
        if (this->currentCommand != COMMANDS)
        {
            this->commands[this->currentCommand]->end(interrupted);
        }
    }

    bool isFinished() const override { return this->currentCommand == COMMANDS; }

private:
    std::array<Command*, COMMANDS> commands;
    size_t currentCommand;
    bool commandInitialized;
};  // class SequentialCommand

}  // namespace control

}  // namespace tap

#endif  // TAPROOT_SEQUENTIAL_COMMAND_HPP_