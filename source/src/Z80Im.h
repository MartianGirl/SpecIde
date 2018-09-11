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

/** Z80Im.h
 *
 * Instruction: IM 0
 * Instruction: IM 1
 * Instruction: IM 2
 *
 */

bool z80Im()
{
    if ((y & 0x02) == 0x00)
        im = 0;
    else if ((y & 0x01) == 0x00)
        im = 1;
    else
        im = 2;

    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
