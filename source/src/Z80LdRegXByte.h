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

/** Z80LdRegXByte.h
 *
 * Instruction: DD-prefixed LD r, n
 *
 * Encoding: 11 011 101  00 rrr 110
 * M Cycles: 3 (DD, OCF, MRB)
 * T States: 11
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IXh 100
 * IXl 101
 *  A  111
 *
 */

bool z80LdRegXByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            *(regx8[y]) = iReg.b.h;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
