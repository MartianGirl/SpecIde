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

/** Z80RlcPtrIxIy.h
 *
 * Instruction: RLC (IX + d)
 * Instruction: RLC (IX + d), r
 * Instruction: RLC (IY + d)
 * Instruction: RLC (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  00 000 rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  00 000 rrr
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
 * - MSB is copied into LSB and into CF.
 *
 */

bool z80RlcPtrIxIy()
{
    switch (executionStep)
    {
        // Previous steps are executed by the prefix.
        case 5:
            return false;

        case 6:
            af.b.l = flg = rlcFlags[iReg.b.h];
            acc.w = iReg.b.h << 1;
            oReg.b.l = acc.b.l | acc.b.h;

            if (z != 6)
                *reg8[z] = oReg.b.l;
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
