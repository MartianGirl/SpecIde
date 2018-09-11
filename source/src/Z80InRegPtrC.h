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

/** Z80InRegPtrC.h
 *
 * Instruction: IN r, (C)
 *
 * Encoding: 11 101 101  01 rrr 000
 * M Cycles: 3 (ED, OCF, IORD)
 * T States: 12
 *
 *  Reg  r
 *  --- ---
 *   B  000
 *   C  001
 *   D  010
 *   E  011
 *   H  100
 *   L  101
 *   -  110     * Only updates the flags, drops the result.
 *   A  111
 *
 * Flags: SZ503P0*
 *
 */

bool z80InRegPtrC()
{
    switch (executionStep)
    {
        case 0:
            ioRdCycles = 1;
            memAddrMode = 0x0000000E;
            wz.w = bc.w;
            return true;

        case 1:
            acc.h = acc.l = iReg.h;

            acc.h ^= acc.h >> 1;
            acc.h ^= acc.h >> 2;
            acc.h ^= acc.h >> 4;

            af.l &= FLAG_C;
            af.l |= (FLAG_S | FLAG_5 | FLAG_3) & acc.l;
            af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
            af.l |= (acc.l) ? 0x00 : FLAG_Z;

            if (y != 6)
                *reg8[y] = acc.l;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
