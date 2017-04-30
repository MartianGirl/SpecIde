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

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rld : public Z80Instruction
{
    public:
        Z80Rld() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->cpuProcCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000022;
                    return true;

                case 1:
                    r->acc.w = r->iReg.h;
                    return true;

                case 2:
                    r->acc.w <<= 4;
                    r->acc.w |= r->af.h & 0x0F;
                    return false;

                case 3:
                    r->af.h &= 0xF0;
                    r->af.h |= r->acc.h & 0x0F;
                    return false;

                case 4:
                    r->acc.h = r->af.h;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l &= FLAG_C;
                    r->af.l |= r->af.h & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= r->af.h ? 0x00 : FLAG_Z;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;
                    return false;

                case 5:
                    r->oReg.l = r->acc.l;
                    return true;

                case 6:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
