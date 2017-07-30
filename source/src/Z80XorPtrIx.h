#pragma once

/** Z80XorPtrIx.h
 *
 * Instruction: XOR (IX+d)
 *
 */

bool z80XorPtrIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000061;
            return true;

        case 1:
            cpuProcCycles = 1;
            return true;

        case 2:
            tmp.l = iReg.h;
            return false;

        case 3:
            tmp.h = ((tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 4:
            tmp.w += ix.w;
            return false;

        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            // Calculate the result.
            acc.l = acc.h = af.h ^ iReg.h;
            acc.h ^= acc.h >> 1;
            acc.h ^= acc.h >> 2;
            acc.h ^= acc.h >> 4;
            af.l = (acc.h & 0x01) ? 0x00 : FLAG_PV;   // ...0.P00
            af.l |=
                acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.503P00
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ503P00
            af.h = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
