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

/** Z80Rrd.h
 *
 * Instruction: RRD
 *
 * Encoding: 11 101 101  01 100 111
 * M Cycles: 5 (OCF, OCF, MRB, CPU(4), MWB)
 * T States: 18
 *
 * Flags: SZ503P0*
 *
 */

bool z80Rrd()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000082;
            return true;

        case 1:
            wz.w = hl.w;
            acc.w = iReg.h;
            return true;

        case 2:
            acc.h = af.h & 0x0F;
            af.h &= 0xF0;
            af.h |= acc.l & 0x0F;
            return false;

        case 3:
            acc.w >>= 4;
            return false;

        case 4:
            acc.h = af.h;
            acc.h ^= acc.h >> 1;
            acc.h ^= acc.h >> 2;
            acc.h ^= acc.h >> 4;
            af.l &= FLAG_C;
            af.l |= af.h & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= af.h ? 0x00 : FLAG_Z;
            af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
            return false;

        case 5:
            oReg.l = acc.l;
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
