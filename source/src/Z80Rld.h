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

/** Z80Rld.h
 *
 * Instruction: RLD
 *
 * Encoding: 11 101 101  01 101 111
 * M Cycles: 5 (OCF, OCF, MRB, CPU(4), MWB)
 * T States: 18
 *
 * Flags: SZ503P0*
 *
 */

bool z80Rld()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000082;
            return true;

        case 1:
            wz.w = hl.w;
            acc.w = iReg.b.h;

            cpuProcCycles = 1;
            skipCycles = 3;
            return true;

        case 2:
        case 3:
        case 4:
            return false;

        case 5:
            acc.w <<= 4;
            acc.w |= af.b.h & 0x0F;

            af.b.h &= 0xF0;
            af.b.h |= acc.b.h & 0x0F;

            acc.b.h = af.b.h;
            acc.b.h ^= acc.b.h >> 1;
            acc.b.h ^= acc.b.h >> 2;
            acc.b.h ^= acc.b.h >> 4;
            flg = af.b.l & FLAG_C;
            flg |= af.b.h & (FLAG_S | FLAG_5 | FLAG_3);
            flg |= af.b.h ? 0x00 : FLAG_Z;
            flg |= (acc.b.h & 0x01) ? 0x00 : FLAG_PV;
            af.b.l = flg;

            oReg.b.l = acc.b.l;

            memWrCycles = 1;
            return true;

        case 6:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
