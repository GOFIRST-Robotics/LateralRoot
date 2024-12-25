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

#ifndef TAPROOT_COMMAND_GOVERNOR_INTERFACE_MOCK_HPP_
#define TAPROOT_COMMAND_GOVERNOR_INTERFACE_MOCK_HPP_

#include <gmock/gmock.h>

#include "tap/control/governor/command_governor_interface.hpp"

namespace tap::mock
{
class CommandGovernorInterfaceMock : public tap::control::governor::CommandGovernorInterface
{
public:
    CommandGovernorInterfaceMock();
    ~CommandGovernorInterfaceMock();
    MOCK_METHOD(bool, isReady, (), (override));
    MOCK_METHOD(bool, isFinished, (), (override));
};
}  // namespace tap::mock

#endif  // TAPROOT_COMMAND_GOVERNOR_INTERFACE_MOCK_HPP_
