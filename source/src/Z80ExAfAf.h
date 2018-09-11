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

/** Z80ExAfAf.h
 *
 * Instruction: EX AF, AF'
 *
 * Encoding: 00 001 000
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80ExAfAf()
{
    acc.w = af.w;
    af.w = af_.w;
    af_.w = acc.w;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
