# Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
#
# This file is part of Taproot.
#
# Taproot is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Taproot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Taproot.  If not, see <https://www.gnu.org/licenses/>.

from SCons.Script import *


CMD_LINE_ARGS                       = 1
TEST_BUILD_TARGET_ACCEPTED_ARGS     = ["build-tests", "run-tests", "run-tests-gcov"]
SIM_BUILD_TARGET_ACCEPTED_ARGS      = ["build-sim", "run-sim"]
HARDWARE_BUILD_TARGET_ACCEPTED_ARGS = ["build", "run", "size", "gdb"]
VALID_BUILD_PROFILES                = ["debug", "release", "fast"]
VALID_PROFILING_TYPES               = ["true", "false"]

USAGE = "Usage: scons <target> [profile=<debug|release|fast>] [profiling=<true|false>] [test=\"<test>\"]\n\
    \"<target>\" is one of:\n\
        - \"build\": build all code for the hardware platform.\n\
        - \"run\": build all code for the hardware platform, and deploy it to the board via a connected ST-Link.\n\
        - \"size\": build all code for the hardware platform, and display build size information.\n\
        - \"gdb\": build all code for the hardware platform, opens a gdb session.\n\
        - \"build-tests\": build core code and tests for the current host platform.\n\
        - \"run-tests\": build core code and tests for the current host platform, and execute them locally with the test runner.\n\
        - \"run-tests-gcov\": builds core code and tests, executes them locally, and captures and prints code coverage information\n\
        - \"build-sim\": build all code for the simulated environment, for the current host platform.\n\
        - \"run-sim\": build all code for the simulated environment, for the current host platform, and execute the simulator locally."


def parse_args():
    args = {
        "TARGET_ENV": "",
        "BUILD_PROFILE": "",
        "PROFILING": ""
    }
    if len(COMMAND_LINE_TARGETS) > CMD_LINE_ARGS:
        raise Exception("You did not enter the correct number of arguments.\n" + USAGE)

    # Extract the target environment from the first command line argument
    # and determine modm build path as well as add any extra files to ignore
    if len(COMMAND_LINE_TARGETS) != 0:
        build_target = COMMAND_LINE_TARGETS[0]
        if build_target == "help":
            print(USAGE)
            exit(0)
        elif build_target in TEST_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "tests"
        elif build_target in SIM_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "sim"
        elif build_target in HARDWARE_BUILD_TARGET_ACCEPTED_ARGS:
            args["TARGET_ENV"] = "hardware"
        else:
            raise Exception("You did not select a valid target.\n" + USAGE)
    else:
        raise Exception("You must select a valid robot target.\n" + USAGE)

    # Extract and validate the build profile (either debug or release)
    default_build_profile = "debug" if args["TARGET_ENV"] == "tests" else "release"
    args["BUILD_PROFILE"] = ARGUMENTS.get("profile", default_build_profile)
    ARGUMENTS["profile"] = args["BUILD_PROFILE"]
    if args["BUILD_PROFILE"] not in VALID_BUILD_PROFILES:
        raise Exception("You specified an invalid build profile.\n" + USAGE)

    args["PROFILING"] = ARGUMENTS.get("profiling", "false")
    if args["PROFILING"] not in VALID_PROFILING_TYPES:
        raise Exception("You specified an invalid profiling type.\n" + USAGE)

    if "test" in ARGUMENTS:
        args["TEST"] = ARGUMENTS.get("test", None)

    return args
