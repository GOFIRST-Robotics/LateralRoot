/*
 * Copyright (c) 2025 Advanced Robotics at the University of Washington <robomstr@uw.edu>
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

#ifndef TAPROOT_SH1107_HPP_
#error "Don't include this file directly, use 'sh1107.hpp' instead!"
#endif

#include "sh1107_defines.hpp"

namespace details
{
inline void rotateBox(uint8_t *box)
{
    uint8_t temp[8] = {0};

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            temp[i] |= ((box[j] >> i) & 1) << (8 - j - 1);
        }
    }

    for (int i = 0; i < 8; i++) box[i] = temp[i];
}

inline void rotateMatrix(
    int Height,
    int Width,
    uint8_t (&matrix)[16][128],
    uint8_t (&rotatedMatrix)[16][128])
{
    for (int i = 0; i < Height / 8; i++)
    {
        for (int j = 0; j < Width / 8; j++)
        {
            uint8_t targetBytes[8];
            for (int k = 0; k < 8; k++)
            {
                targetBytes[k] = matrix[j][i * 8 + 7 - k];
            }

            rotateBox(targetBytes);

            for (int k = 0; k < 8; k++)
            {
                rotatedMatrix[i][j * 8 + k] = targetBytes[k];
            }
        }
    }

    for (int i = 0; i < Height / 8; i++)
    {
        for (int j = 0; j < Width / 2; j++)
        {
            uint8_t tmp = rotatedMatrix[i][j];
            rotatedMatrix[i][j] = rotatedMatrix[i][Width - 1 - j];
            rotatedMatrix[i][Width - 1 - j] = tmp;
        }
    }
}
}  // namespace details

template <
    typename SPI,
    typename A0,
    typename Reset,
    unsigned int Width,
    unsigned int Height,
    bool Flipped,
    bool Rotate>
modm::ResumableResult<bool> tap::display::Sh1107<SPI, A0, Reset, Width, Height, Flipped, Rotate>::
    updateNonblocking()
{
    RF_BEGIN(0);

    if (!writeToDisplay.testAndSet(false)) RF_RETURN(false);

    if (Rotate)
    {
        details::rotateMatrix(Height, Width, this->buffer, rotatedMatrix);
    }

    for (y = 0; y < (Height / 8); ++y)
    {
        // command mode
        a0.reset();
        RF_CALL(spi.transfer(SH1107_PAGE_ADDRESS | y));  // Row select
        RF_CALL(spi.transfer(SH1107_COL_ADDRESS_MSB));   // Column select high
        RF_CALL(spi.transfer(SH1107_COL_ADDRESS_LSB));   // Column select low

        // switch to data mode
        a0.set();
        for (x = 0; x < Width; ++x)
        {
            if (Rotate)
            {
                RF_CALL(spi.transfer(rotatedMatrix[y][x]));
            }
            else
            {
                RF_CALL(spi.transfer(this->buffer[y][x]));
            }
        }
    }
    a0.reset();

    RF_END_RETURN(true);
}

template <
    typename SPI,
    typename A0,
    typename Reset,
    unsigned int Width,
    unsigned int Height,
    bool Flipped,
    bool Rotate>
void tap::display::Sh1107<SPI, A0, Reset, Width, Height, Flipped, Rotate>::update()
{
    writeToDisplay.testAndSet(true);
}

template <
    typename SPI,
    typename A0,
    typename Reset,
    unsigned int Width,
    unsigned int Height,
    bool Flipped,
    bool Rotate>
void tap::display::Sh1107<SPI, A0, Reset, Width, Height, Flipped, Rotate>::setInvert(bool invert)
{
    a0.reset();

    if (invert)
    {
        spi.transferBlocking(SH1107_REVERSE);
    }
    else
    {
        spi.transferBlocking(SH1107_NORMAL);
    }
}

// ----------------------------------------------------------------------------
template <
    typename SPI,
    typename A0,
    typename Reset,
    unsigned int Width,
    unsigned int Height,
    bool Flipped,
    bool Rotate>
void tap::display::Sh1107<SPI, A0, Reset, Width, Height, Flipped, Rotate>::initializeBlocking()
{
    a0.setOutput();
    reset.setOutput();
    modm::delay_ms(20);

    // reset the controller
    reset.set();
    modm::delay_ms(1);
    reset.reset();
    modm::delay_ms(20);
    reset.set();

    a0.reset();
    modm::delay_ms(20);

    if (Flipped)
    {
        spi.transferBlocking(SH1107_ADC_NORMAL);
        spi.transferBlocking(SH1107_SCAN_DIR_NORMAL);
    }
    else
    {
        spi.transferBlocking(SH1107_ADC_REVERSE);
        spi.transferBlocking(SH1107_SCAN_DIR_REVERSE);
    }

    spi.transferBlocking(SH1107_ON);

    this->clear();
    this->update();
}
