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

/** Z80JrByte.h
 *
 * JR n Instruction.
 *
 */

bool z80JrByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            cpuProcCycles = 1;
            skipCycles = 4;
            return true;

        case 2:
        case 3:
        case 4:
        case 5:
            return false;

        case 6:
            wz.b.l = iReg.b.h;
            wz.b.h = ((wz.b.l & 0x80) == 0x80) ? 0xFF : 0x00;
            wz.w += pc.w;
            pc.w = wz.w;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
