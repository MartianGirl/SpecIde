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

/** Z80RetCc.h
 *
 * Instruction: RET NZ
 * Instruction: RET Z
 * Instruction: RET NC
 * Instruction: RET C
 * Instruction: RET PO
 * Instruction: RET PE
 * Instruction: RET P
 * Instruction: RET M
 *
 */

bool z80RetCc()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x000000BB;
            return false;

        case 1:
            switch (y)
            {
                case 0: memRdCycles = ((af.b.l & FLAG_Z) == 0x00) ? 2 : 0; break;
                case 1: memRdCycles = ((af.b.l & FLAG_Z) == FLAG_Z) ? 2 : 0; break;
                case 2: memRdCycles = ((af.b.l & FLAG_C) == 0x00) ? 2 : 0; break;
                case 3: memRdCycles = ((af.b.l & FLAG_C) == FLAG_C) ? 2 : 0; break;
                case 4: memRdCycles = ((af.b.l & FLAG_PV) == 0x00) ? 2 : 0; break;
                case 5: memRdCycles = ((af.b.l & FLAG_PV) == FLAG_PV) ? 2 : 0; break;
                case 6: memRdCycles = ((af.b.l & FLAG_S) == 0x00) ? 2 : 0; break;
                case 7: memRdCycles = ((af.b.l & FLAG_S) == FLAG_S) ? 2 : 0; break;
                default: assert(false); break;
            }
            return true;

        case 2:
            return true;

        case 3:
            pc.w = wz.w = iReg.w;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
