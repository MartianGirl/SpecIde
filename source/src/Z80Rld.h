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
            cpuProcCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.w = iReg.h;
            return true;

        case 2:
            acc.w <<= 4;
            acc.w |= af.h & 0x0F;
            return false;

        case 3:
            af.h &= 0xF0;
            af.h |= acc.h & 0x0F;
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
