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

/** Z80ResNPtrHl.h
 *
 * Instruction: RES n, (HL)
 *
 * Encoding: 11 001 011  10 bbb 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 */

bool z80ResNPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.b.l = iReg.b.h;
            acc.b.l &= ~(1 << y);
            return false;

        case 2:
            oReg.b.l = acc.b.l;
            return true;

        case 3:
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
