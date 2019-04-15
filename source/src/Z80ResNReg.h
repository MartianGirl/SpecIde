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

/** Z80ResNReg.h
 *
 * Instruction: RES n, r
 *
 * Encoding: 11 001 011  10 bbb rrr
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

bool z80ResNReg()
{
    *reg8[z] &= ~(1 << y);

    flg = 0;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
