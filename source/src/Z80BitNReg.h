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

/** Z80BitNReg.h
 *
 * Instruction: BIT n, r
 *
 * Encoding: 11 001 011  01 bbb rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80BitNReg()
{
    acc.b.l = *reg8[z];

    flg = af.b.l & FLAG_C;
    flg |= FLAG_H;
    flg |= acc.b.l & (FLAG_5 | FLAG_3);

    acc.b.l &= (1 << y);
    flg |= acc.b.l & FLAG_S;
    flg |= (acc.b.l != 0x00) ? 0x00 : (FLAG_Z | FLAG_PV);

    af.b.l = flg;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
