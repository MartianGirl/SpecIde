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

/** Z80ExPtrSpHl.h
 *
 * Instruction: EX (SP), HL
 *
 * Encoding: 11 100 011
 * M Cycles: 5 (OCF, SRB(3), SRB(4), SWB(3), SWB(5))
 * T States: 19
 *
 */

bool z80ExPtrSpHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 2;
            memAddrMode = 0x0000AABB;
            return true;

        case 1:
            skipCycles = 1;
            return true;

        case 2:
            return false;

        case 3:
            oReg.b.l = hl.b.h;
            oReg.b.h = hl.b.l;
            return true;

        case 4:
            skipCycles = 2;
            return true;

        case 5:
        case 6:
            return false;

        case 7:
            hl.w = wz.w = iReg.w;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}
// vim: et:sw=4:ts=4
