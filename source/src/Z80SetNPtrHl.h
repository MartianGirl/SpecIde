#pragma once

/** Z80SetNPtrHl.h
 *
 * Instruction: SET n, (HL)
 *
 * Encoding: 11 001 011  11 bbb 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80SetNPtrHl : public Z80Instruction
{
    public:
        Z80SetNPtrHl() {}

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
                    r->acc.l = r->iReg.h;
                    r->acc.l |= (1 << r->y);
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
