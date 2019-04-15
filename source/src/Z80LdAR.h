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

/** Z80LdAR.h
 *
 * Instruction: LD A, R
 *
 * Encoding: 11 101 101  01 011 111
 * M Cycles: 2 (ED, OCF)
 * T States: 9
 *
 * Flags: SZ503P0-*
 *
 * - PF <- IFF2
 *
 */

bool z80LdAR()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            return false;

        case 1:
            af.b.h = ir.b.l;
            flg = af.b.l & FLAG_C;                              // 0000000C
            flg |= (ir.b.l & (FLAG_S | FLAG_5 | FLAG_3));       // S050300C
            flg |= (ir.b.l == 0x00) ? FLAG_Z : 0x00;            // SZ50300C
            flg |= (iff & IFF2);                                // SZ503P0C
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
