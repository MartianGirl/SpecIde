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

/** Z80DecPtrIy.h
 *
 * Instruction: DEC (IY+d)
 *
 */

bool z80DecPtrIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000771;
            return true;

        case 1:
            wz.b.l = iReg.b.h;
            return true;

        case 2:
            wz.b.h = ((wz.b.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += iy.w;
            return false;

        case 4:
        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            // Preserve carry bit.
            flg = af.b.l & FLAG_C;
            flg |= decFlags[iReg.b.h];
            return false;

        case 8:
            oReg.b.l = iReg.b.h - 1;
            memWrCycles = 1;
            return true;

        case 9:
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
