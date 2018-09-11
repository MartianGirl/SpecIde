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

/** Z80Daa.h
 *
 * Instruction: DAA
 *
 */

bool z80Daa()
{
    // Keep the relevant flags.
    af.l &= (FLAG_H | FLAG_N | FLAG_C);          // ...H..NC
    // Adjust the lower nybble first.
    acc.w = (af.h & 0x0F);
    if ((acc.w > 0x09) || ((af.l & FLAG_H) == FLAG_H))
    {
        if ((af.l & FLAG_N) == FLAG_N)   // Subtraction
        {
            af.l &= (acc.w > 0x05) ? ~FLAG_H : 0xFF;
            acc.w -= 0x06;
        }
        else                                // Addition
        {
            af.l &= ~FLAG_H;
            af.l |= (acc.w > 0x09) ? FLAG_H : 0x00;
            acc.w += 0x06;
        }
    }

    // Adjust the upper nybble then.
    acc.w += (af.h & 0xF0);
    if ((af.h > 0x99) || ((af.l & FLAG_C) == FLAG_C))
    {
        if ((af.l & FLAG_N) == FLAG_N)   // Subtraction
        {
            acc.w -= 0x60;
        }
        else                                // Addition
        {
            acc.w += 0x60;
        }

        af.l |= FLAG_C;
    }

    af.l |= 
        acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.5H3.NC
    acc.h = acc.l;
    acc.h ^= acc.h >> 1;
    acc.h ^= acc.h >> 2;
    acc.h ^= acc.h >> 4;
    af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;  // S.5H3PNC
    af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3PNC
    af.h = acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
