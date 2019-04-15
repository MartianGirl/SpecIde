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

/** Z80RlReg.h
 *
 * Instruction: RL r
 *
 * Encoding: 11 001 011  00 010 rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 * Flags: SZ503P0C
 * - MSB is copied into CF. CF is copied into LSB.
 *
 */

bool z80RlReg()
{
    acc.w = (*reg8[z] << 1) | (af.b.l & FLAG_C);
    af.b.l = flg = rlFlags[af.b.l & FLAG_C][*reg8[z]];
    *reg8[z] = acc.b.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
