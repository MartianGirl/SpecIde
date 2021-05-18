/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
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

static uint_fast32_t GateArray::pixelsX1[X_SIZE * Y_SIZE / 2];
static uint_fast32_t GateArray::pixelsX2[X_SIZE * Y_SIZE];


void GateArray::write(uint_fast8_t byte) {

    // 11xx xxxx: RAM memory management (performed externally)
    switch (byte & 0xC0) {
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

uint_fast8_t GateArray::read() {
}

void GateArray::selectPen(uint_fast8_t byte) {

    // Pen 0x10 and higher is for border.
    if (byte & 0x10) {
        pen = 0xFF;
    } else {
        pen = byte & 0x0F;
    }
}

void GateArray::selectColour(uint_fast8_t byte) {

    if (pen == 0xFF) {
        border = byte & 0x1F;
    } else {
        pens[pen] = byte & 0x1F;
    }
}

void GateArray::selectScreenAndRom(uint_fast8_t byte) {

    newMode = byte & 0x03;
    lowerRom = byte & 0x04;
    upperRom = byte & 0x08;
    interrupt = byte & 0x10;
}

void GateArray::clock() {

    counter = (counter + 1) & 0x0F;

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
    // (S1 xor S3) or (S5 xor S7), delayed one clock (has digits 378C)
    // true, false, false, true, true, false, false, true
    // true, false, false, true, true, false, false, true
    //
    // CASAD_
    // Same, but one clock after
    // false, true, true, false, false, false, false, false
    // false, false, false, true, true, false, false, false
    // 
    // CPU_
    // not (S1 and not S7)
    // true, true, true, true, true, true, true, true,
    // true, true, false, false, false, false, false, false
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
    //
    // CCLK_
    // S2 nor S5
    // false, false, false, false, false, false, true, true,
    // true, true, true, false, false, false, false, false
    //
    // (S0 and S5 and RD_)
    // RD_, false, false, false, false, false, false, false,
    // false, false, false, false, false, false, RD_, RD_
    //
    // MWE_
    // not (S0 and S5 and RD_)
    // !RD_, true, true, true, true, true, true, true
    // true, true, true, true, true, true, !RD, !RD
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
    // (delayed x2)
    // false, false, true, true, false, false, false, true,
    // false, false, false, false, true, true, false, false
    //
    // S2
    // true, true, true, false, false, false, false, false
    // false, false, false, true, true, true, true, true
    //
    // (not S4 or S5)
    // true, true, true, true, true, true, true, true,
    // true, true, true, true, true, false, true, true
    //
    // (not S4 or S5) or #MREQ (gated the refresh MREQ)
    // true, true, true, true, true, true, true, true
    // true, true, true, true, true, #MREQ, true, true
    //
    // signal above and S2
    // true, true, true, false, false, false, false, false
    // false, false, false, true, true, #MREQ, true, true
    //
    // ANG gate U712
    // #MREQ, true, true, false, false, false, false, false,
    // false, false, false, true, true, #MREQ, #MREQ, #MREQ,
    //
    // (delayed x1 or delayed x2)
    // false, true, true, true, false, false, true, true
    // false, false, false, true, true, true, false, false
    //
    // #CAS (delayed x1 or delayed x2 or U712)
    // #MREQ, true, true, true, false, false, true, true,
    // false, false, false, true, true, true, #MREQ, #MREQ
    //
    // RAS_
    // S0 and (not S2 or S6)
    // true, true, false, false, false, false, false, false
    // false, false, true, true, false, false, false, false
    //
    // 74LS153 multiplexers (0 = A15-A9,A0, 1 = A8-A1,
    // 2 = M13-M12, RA2-0, M9-M8-CCLK, 3 = M7-M0)
    // 2, 3, 3, 2, 2, 2, 2, 2,
    // 2, 2, 0, 1, 1, 0, 0, 0
    //
    // This means that the frame (starting at 0) is:
    // 0: nothing
    // 1: nothing - wait
    // 2: video row address latched - wait
    // 3: nothing - wait
    // 4: video col address latched - wait
    // 5-6-7: get data - wait
    // 8: video col address latched - wait
    // 9-a-b: get data - wait
    // c: cpu row address latched - go
    // d: nothng
    // e: cpu col address latched
    // f-0: get data
    switch (counter) {
        case 1:
            z80_c &= ~SIGNAL_WAIT_; break;
        case 5:
        case 9:
            videoByte = d; break;
        case c:
            z80_c |= SIGNAL_WAIT_; break;
        default:
            break;
    }
}
// vim: et:sw=4:ts=4
