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

/** Z80RlPtrHl.h
 *
 * Instruction: RL (HL)
 *
 * Encoding: 11 001 011  00 010 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 * Flags: SZ503P0C
 * - MSB is copied into CF. CF is copied into LSB.
 *
 */

bool z80RlPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.w = iReg.b.h << 1;
            acc.w |= af.b.l & FLAG_C;
            flg = acc.b.h & FLAG_C;
            acc.b.h = acc.b.l;
            acc.b.h ^= acc.b.h >> 1;
            acc.b.h ^= acc.b.h >> 2;
            acc.b.h ^= acc.b.h >> 4;
            flg |= acc.b.l & (FLAG_S | FLAG_5 | FLAG_3);
            flg |= (acc.b.l) ? 0x00 : FLAG_Z;
            flg |= (acc.b.h & 0x01) ? 0x00 : FLAG_PV;
            af.b.l = flg;
            return false;

        case 2:
            oReg.b.l = acc.b.l;
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
