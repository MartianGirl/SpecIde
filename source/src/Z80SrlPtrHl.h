#pragma once

/** Z80SrlPtrHl.h
 *
 * Instruction: SRL (HL)
 *
 * Encoding: 11 001 011  00 111 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 * Flags: SZ503P0C
 * - 0 is shifted into MSB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80SrlPtrHl : public Z80Instruction
{
    public:
        Z80SrlPtrHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000022;
                    return true;

                case 1:
                    r->acc.w = r->iReg.h;
                    r->af.l = r->acc.l & FLAG_C;
                    r->acc.w >>= 1;
                    r->acc.h = r->acc.l;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;
                    return false;

                case 2:
                    r->oReg.l = r->acc.l;
                    return true;

                case 3:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
