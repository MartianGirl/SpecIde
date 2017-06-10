#pragma once

/** Z80IncPtrHl.h
 *
 * Instruction: INC (HL)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80IncPtrHl : public Z80Instruction
{
    public:
        Z80IncPtrHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000022;
                    return true;

                case 1:
                    r->acc.w = r->iReg.h + 1;

                    // Preserve carry flag.
                    r->af.l &= FLAG_C;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l ^ r->iReg.h) & FLAG_H;
                    r->af.l |= (((r->acc.l ^ r->iReg.h) >> 5) 
                            ^ (r->acc.h << 2)) & FLAG_PV;
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
                    return false;

                case 2:
                    r->oReg.l = r->acc.l;
                    r->memWrCycles = 1;
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
