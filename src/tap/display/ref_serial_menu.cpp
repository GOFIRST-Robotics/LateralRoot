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

#include "ref_serial_menu.hpp"

#include "tap/drivers.hpp"

using namespace tap::communication::serial;

namespace tap::display
{
RefSerialMenu::RefSerialMenu(
    modm::ViewStack<DummyAllocator<modm::IAbstractView> >* stack,
    Drivers* drivers)
    : modm::AbstractMenu<DummyAllocator<modm::IAbstractView> >(stack, 1),
      drivers(drivers),
      verticalScroll(drivers, REF_SERIAL_INFO_LINES, DISPLAY_MAX_ENTRIES),
      printRefSerialDataFncPtrs{
          &RefSerialMenu::printRobotTypeId,
          &RefSerialMenu::printHp,
          &RefSerialMenu::print17mm1Heat,
          &RefSerialMenu::print17mm2Heat,
          &RefSerialMenu::print42mmHeat,
          &RefSerialMenu::printPowerBuf,
          &RefSerialMenu::printPower,
      }
{
}

void RefSerialMenu::draw()
{
    modm::GraphicDisplay& display = getViewStack()->getDisplay();
    display.clear();
    display.setCursor(0, 2);
    display << getMenuName() << modm::endl;

    for (int8_t i = verticalScroll.getSmallestIndexDisplayed();
         i <= verticalScroll.getLargestIndexDisplayed();
         i++)
    {
        display << (i == verticalScroll.getCursorIndex() ? "> " : "  ");
        (this->*printRefSerialDataFncPtrs[i])(display);
    }
}

void RefSerialMenu::shortButtonPress(modm::MenuButtons::Button button)
{
    if (button == modm::MenuButtons::LEFT)
    {
        this->remove();
    }
    else
    {
        verticalScroll.onShortButtonPress(button);
    }
}

void RefSerialMenu::update() {}

bool RefSerialMenu::hasChanged()
{
    return verticalScroll.acknowledgeCursorChanged() || updatePeriodicTimer.execute();
}

void RefSerialMenu::printRobotTypeId(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "Robot ID: " << static_cast<int>(robotData.robotId) << ", "
           << (RefSerialData::isBlueTeam(robotData.robotId) ? "BLUE" : "RED") << modm::endl;
}

void RefSerialMenu::printHp(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "HP: " << robotData.currentHp << "/" << robotData.maxHp << modm::endl;
}

void RefSerialMenu::print17mm1Heat(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "17mm1Heat: " << robotData.turret.heat17ID1 << " / " << robotData.turret.heatLimit
           << modm::endl;
}

void RefSerialMenu::print17mm2Heat(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "17mm2Heat: " << robotData.turret.heat17ID2 << " / " << robotData.turret.heatLimit
           << modm::endl;
}

void RefSerialMenu::print42mmHeat(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "42mmHeat: " << robotData.turret.heat42 << " / " << robotData.turret.heatLimit
           << modm::endl;
}

void RefSerialMenu::printPowerBuf(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream << "PowerBuf: " << robotData.chassis.powerBuffer << modm::endl;
}

void RefSerialMenu::printPower(modm::IOStream& stream)
{
    const auto& robotData = drivers->refSerial.getRobotData();
    stream.printf("Power limit: %i", robotData.chassis.powerConsumptionLimit);
}

}  // namespace tap::display
