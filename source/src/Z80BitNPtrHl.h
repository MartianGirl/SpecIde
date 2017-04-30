#pragma once

/** Z80BitNPtrHl.h
 *
 * Instruction: BIT n, (HL)
 *
 * Encoding: 11 001 011  01 bbb 110
 * M Cycles: 2 (OCF, OCF, MRB(4))
 * T States: 12
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80BitNPtrHl : public Z80Instruction
{
    public:
        Z80BitNPtrHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000002;
                    return true;

                case 1:
                    r->acc.l = r->iReg.h;
                    r->acc.l &= (1 << r->y);

                    r->af.l &= FLAG_C;
                    r->af.l |= FLAG_H;
                    r->af.l |= r->acc.l & FLAG_S;
                    r->af.l |= r->tmp.h & (FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z | FLAG_PV;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
