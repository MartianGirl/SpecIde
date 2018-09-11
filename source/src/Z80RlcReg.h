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

/** Z80RlcReg.h
 *
 * Instruction: RLC r
 *
 * Encoding: 11 001 011  00 000 rrr
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
 * - MSB is copied into LSB and into CF.
 *
 */

bool z80RlcReg()
{
    acc.w = *reg8[z] << 1;
    acc.h = acc.l = acc.l | acc.h;
    *reg8[z] = acc.l;
    acc.h ^= acc.h >> 1;
    acc.h ^= acc.h >> 2;
    acc.h ^= acc.h >> 4;
    af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3 | FLAG_C);
    af.l |= (acc.l) ? 0x00 : FLAG_Z;
    af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
