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

/** Z80Ccf.h
 *
 * Instruction: CCF
 *
 */

bool z80Ccf()
{
    af.l &= (FLAG_S | FLAG_Z | FLAG_PV | FLAG_C);
    af.l |= af.h & (FLAG_5 | FLAG_3);
    af.l |= ((af.l & FLAG_C) << 4) & FLAG_H;
    af.l ^= FLAG_C;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
