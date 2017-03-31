#pragma once

/** Z80RetI.h
 *
 * Instruction: RETI
 * Instruction: RETN
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80RetI : public Z80Instruction
{
    public:
        Z80RetI() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x000000BB;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->iff = ((r->iff & IFF2) >> 2) | (r->iff & IFF2);
                    r->pc.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
