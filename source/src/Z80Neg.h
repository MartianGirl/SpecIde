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

/** Z80Neg.h
 *
 * Instruction: NEG
 *
 */

bool z80Neg()
{
    // Calculate half-carry. This is done by doing a 4-bit
    // subtraction. Half-carry will be in bit 4.
    acc.w = -(af.b.h & 0x0F);
    flg = (acc.w & (FLAG_H | FLAG_3)) | FLAG_N;    // ...b.H3.1.  

    // Calculate carry in the bit 7. Overflow flag is
    // (carry in bit 7) XOR (carry in bit 8).
    acc.w = -(af.b.h & 0x7F);
    flg |= (acc.w >> 5) & FLAG_PV;

    // Calculate the result.
    acc.w = -af.b.h;
    flg |= acc.b.l & (FLAG_S | FLAG_5);         // S.5H3.1.
    flg |= acc.b.h & FLAG_C;                    // S.5H3.1C
    flg ^= (acc.w >> 6) & FLAG_PV;              // S.5H3V1C
    flg |= (acc.b.l) ? 0x00 : FLAG_Z;           // SZ5H3V1C
    af.b.h = acc.b.l;
    af.b.l = flg;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
