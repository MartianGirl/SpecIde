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

/** Z80Rrca.h
 *
 * Instruction: RRCA
 *
 * Encoding: 00 001 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - LSB is copied into MSB and into CF.
 *
 */

bool z80Rrca()
{
    acc.h = af.h & 0x01;
    acc.l = af.h;
    af.l &= FLAG_S | FLAG_Z | FLAG_PV;
    af.l |= acc.h & FLAG_C;
    acc.w >>= 1;
    af.h = acc.l;
    af.l |= af.h & (FLAG_5 | FLAG_3);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
