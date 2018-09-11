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

/** Z80SbcReg.h
 *
 * Instruction: SBC A, r
 *
 */

bool z80SbcReg()
{
    acc.w = af.h - *reg8[z];
    acc.w -= af.l & FLAG_C;

    af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3);
    af.l |= FLAG_N;
    af.l |= (acc.l ^ *reg8[z] ^ af.h) & FLAG_H;
    af.l |= (((acc.l ^ *reg8[z] ^ af.h) >> 5) 
            ^ (acc.h << 2)) & FLAG_PV;
    af.l |= acc.h & FLAG_C;                   // S.5H3V0C
    af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
    af.h = acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
