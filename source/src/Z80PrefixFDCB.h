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

#pragma once

/** Z80PrefixFDCB.h
 *
 * Prefix FDCB.
 *
 */

bool z80PrefixFDCB()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00007711;
            return true;

        case 1: // This is the displacement.
            wz.l = iReg.h;
            return true;

        case 2: // This is actually the opcode. Now we *decode* it,
            // as if we were starting a new instruction.
            // Also, during this cycle we calculate the operand
            // address.
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += iy.w;
            return false;

        case 4:
            opcode = iReg.h;
            x = (iReg.h & 0xC0) >> 6; // xx......
            y = (iReg.h & 0x38) >> 3; // ..yyy...
            z = (iReg.h & 0x07);      // .....zzz
            p = y >> 1;               // ..pp....
            q = y & 0x01;             // ....q...

            memRdCycles = 1;
            memWrCycles = (x == 1) ? 0 : 1;
            ioRdCycles = 0;
            ioWrCycles = 0;
            cpuProcCycles = 0;

            prefix = PREFIX_FD | PREFIX_CB;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}
// vim: et:sw=4:ts=4
