#pragma once

/** Z80IntMode2.h
 *
 * Interrupt mode 2 process (modelled as an instruction!)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80IntMode2 : public Z80Instruction
{
    public:
        Z80IntMode2() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x0000AA9C;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->acc.w = r->pc.w;
                    return false;

                case 3:
                    r->oReg.l = r->acc.h;
                    r->oReg.h = r->acc.l;
                    return true;

                case 4:
                    return true;

                case 5:
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
