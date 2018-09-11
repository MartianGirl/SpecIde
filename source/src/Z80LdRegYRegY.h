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

/** Z80LdRegYRegY.h
 *
 * Instruction: LD r, s (FD-Prefixed)
 *
 * Encoding: 11 111 101  01 rrr sss
 * M Cycles: 2 (FD, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IYh 100
 * IYl 101
 *  A  111
 *
 */

bool z80LdRegYRegY()
{
    *(regy8[y]) = *(regy8[z]);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
