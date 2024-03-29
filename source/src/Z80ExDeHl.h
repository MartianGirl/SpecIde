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

/** Z80ExDeHl.h
 *
 * Instruction: EX DE, HL
 *
 * Encoding: 11 101 011
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80ExDeHl()
{
    acc.w = de.w;
    de.w = hl.w;
    hl.w = acc.w;

    flg = 0;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
