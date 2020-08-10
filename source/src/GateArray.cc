/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "GateArray.h"

#if SPECIDE_BYTE_ORDER == 1
uint32_t GateArray::colours[32] = {
    0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF,
    0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
    0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF,
    0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
    0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF,
    0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
    0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF,
    0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF};
#else
uint32_t GateArray::colours[32] = {
    0xFF7F7F7F, 0xFF7F7F7F, 0xFF7FFF00, 0xFF7FFFFF,
    0xFF7F0000, 0xFF7F00FF, 0xFF7F7F00, 0xFF7F7FFF,
    0xFF7F00FF, 0xFF7FFFFF, 0xFF00FFFF, 0xFFFFFFFF,
    0xFF0000FF, 0xFFFF00FF, 0xFF007FFF, 0xFFFF7FFF,
    0xFF7F0000, 0xFF7FFF00, 0xFF00FF00, 0xFFFFFF00,
    0xFF000000, 0xFFFF0000, 0xFF007F00, 0xFFFF7F00,
    0xFF7F007F, 0xFF7FFF7F, 0xFF00FF7F, 0xFFFFFF7F,
    0xFF00007F, 0xFFFF007F, 0xFF007F7F, 0xFFFF7F7F};
#endif

static uint_fast32_t GateArray::pixelsX1[0x70000];
static uint_fast32_t GateArray::pixelsX2[0x70000];

uint_fast8_t GateArray::sequence[16] = {
    0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};

// !(S2 | S5) => ((sequence & 0x24) != 0x00)

void GateArray::write(uint_fast8_t byte)
{
    // 11xx xxxx: RAM memory management (performed externally)
    switch (byte & 0xC0)
    {
        case 0x00:  // 00xx xxxx : Select pen
            selectPen(byte);
            break;

        case 0x40:  // 01xx xxxx : Select colour
            selectColour(byte);
            break;

        case 0x80:  // 10xx xxxx : Select screen mode and ROM configuration
            selectScreenAndRom(byte);
            break;

        default:
            break;
    }
}

uint_fast8_t GateArray::read()
{
}

void GateArray::selectPen(uint_fast8_t byte)
{
    if (byte & 0x10)    // Select border
        pen = 0xFF;
    else                // Select pen
        pen = byte & 0x0F;
}

void GateArray::selectColour(uint_fast8_t byte)
{
    if (pen == 0xFF)
        border = byte & 0x1F;
    else
        pens[pen] = byte & 0x1F;
}

void GateArray::selectScreenAndRom(uint_fast8_t byte)
{
    newMode = byte & 0x03;
    lowerRom = byte & 0x04;
    upperRom = byte & 0x08;
    interrupt = byte & 0x10;
}

void GateArray::clock()
{
    counter = (counter + 1) & 0x0F;

    // CRTC clock, edge and level
    // Edge is used to trigger the CRTC. Level is used to generate addresses.
    // Actually, cClk = !(sequence[counter] & 0x24) but this happens from 0x06
    // to 0x0A.
    cClkEdge = (counter == 0x0B);
    cClk = (counter > 0x05 && counter < 0x0B);

    // 1. Sequencer works using a 8-bit shift register to generate the
    // following sequence:
    // 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
    // 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
    //
    // The above sequence gets restarted to 0xFF whenever the Z80 acknowledges
    // an interrupt (M1_, IORQ_ and RD_ are low, RESET is high).
    //
    // The register is clocked at 16MHz.
    //
    // 2. From here, we derive the following signals:
    //
    // PHI_ (4 MHz clock)
    // (S1 xor S3) or (S5 xor S7), one clock later
    // true, false, false, true, true, false, false, true
    // true, false, false, true, true, false, false, true
    //
    // RAS_
    // S0 and (not S2 or S6)
    // true, true, false, false, false, false, false, false
    // false, false, true, true, false, false, false, false
    //
    // CASAD_
    // Same, but one clock after
    // false, true, true, false, false, false, false, false
    // false, false, false, true, true, false, false, false
    //
    // S3 and not S6
    // false, false, false, false, false, false, false, false
    // false, false, false, false, true, true, true, false
    //
    // CASAD
    // true, false, false, true, true, true, true, true
    // true, true, true, false, false, true, true, true
    //
    // READY
    // (CASAD and READY) or (S3 and not S6)
    // true, false, false, false, false, false, false, false
    // false, false, false, false, true, true, true, true
    //
    // CPU_
    // not (S1 and not S7)
    // true, true, true, true, true, true, true, true,
    // true, true, false, false, false, false, false, false
    //
    // CCLK_
    // not (S2 or S5)
    // false, false, false, false, false, false, true, true,
    // true, true, true, false, false, false, false, false
    //
    // MWE_
    // not (S0 and S5 and RD_)
    // false, true, true, true, true, true, true, true
    // true, true, true, true, true, true, false, false
    //
    // 244E_
    // not (S2 and S3 and not IORQ_)
    // false, false, false, true, true, true, true, true
    // true, true, true, true, false, false, false, false
    //
    // INKEN (Gate Array enable)
    // (S0 and S7)
    // true, false, false, false, false, false, false, false
    // false, false, false, false, false, false, false, false
    //
    //
    // (not S4 and S5)
    // false, false, false, false, false, true, false, false
    // false, false, false, false, false, false, false, false
    //
    // (not S3 and S1)
    // false, false, false, false, false, false, false, false
    // false, false, true, true, false, false, false, false
    //
    // (S1 and S7)
    // true, true, false, false, false, false, false, false
    // false, false, false, false, false, false, false, false
    //
    // (not S4 and S5) or (not S3 and S1) or (S1 and S7)
    // true, true, false, false, false, true, false, false
    // false, false, true, true, false, false, false, false
    //
    // (delayed x1)
    // false, true, true, false, false, false, true, false,
    // false, false, false, true, true, false, false, false
    //
    // S2
    // true, true, true, false, false, false, false, false
    // false, false, false, true, true, true, true, true
    //
    // (not S4 or S5)
    // true, true, true, true, true, true, true, true,
    // true, true, true, true, true, false, true, true
    //
    // (delayed x2)
    // false, false, true, true, false, false, false, true
    // false, false, false, false, true, true, false, false
    //
    // (delayed x1 or delayed x2)
    // false, true, true, true, false, false, true, true
    // false, false, false, true, true, true, false, false
    //

}

// vim: et:sw=4:ts=4
