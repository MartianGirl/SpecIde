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

/** Z80SrlPtrIxIy.h
 *
 * Instruction: SRL (IX + d)
 * Instruction: SRL (IX + d), r
 * Instruction: SRL (IY + d)
 * Instruction: SRL (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  00 111 rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  00 111 rrr
 * M Cycles: 6 (OCF, OCF, MRB(5), MRB, MRB(4), MWB)
 * T States: 23
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  -  110
 *  A  111
 *
 * Flags: SZ503P0C
 * - 0 is shifted into MSB.
 *
 */

bool z80SrlPtrIxIy()
{
    switch (executionStep)
    {
        // Previous steps are executed by the prefix.
        case 5:
            return false;

        case 6:
            af.b.l = flg = srlFlags[iReg.b.h];
            acc.w = iReg.b.h >> 1;

            if (z != 6)
                *reg8[z] = acc.b.l;

            oReg.b.l = acc.b.l;
            return true;

        case 7:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
