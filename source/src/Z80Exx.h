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

/** Z80Exx.h
 *
 * Instruction: EXX
 *
 * Encoding: 11 011 001
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80Exx()
{
    tmp.w = bc.w;
    bc.w = bc_.w;
    bc_.w = tmp.w;

    tmp.w = de.w;
    de.w = de_.w;
    de_.w = tmp.w;

    tmp.w = hl.w;
    hl.w = hl_.w;
    hl_.w = tmp.w;

    flg = 0;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
