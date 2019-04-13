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

/** Z80BitNPtrIxIy.h
 *
 * Instruction: BIT n, (IX + d)
 * Instruction: BIT n, (IY + d)
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  01 bbb rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  01 bbb rrr
 * M Cycles: 5 (OCF, OCF, MRB, MRB(5), MRB(4))
 * T States: 20
 *
 */

bool z80BitNPtrIxIy()
{
    switch (executionStep)
    {
        case 5:
            acc.b.l = iReg.b.h;
            acc.b.l &= (1 << y);

            flg = af.b.l & FLAG_C;
            flg |= FLAG_H;
            flg |= acc.b.l & FLAG_S;
            flg |= wz.b.h & (FLAG_5 | FLAG_3);
            flg |= (acc.b.l) ? 0x00 : (FLAG_Z | FLAG_PV);
            return false;

        case 6:
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
