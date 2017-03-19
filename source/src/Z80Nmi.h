#pragma once

/** Z80Nmi.h
 *
 * NMI process (modelled as an instruction!)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Nmi : public Z80Instruction
{
    public:
        Z80Nmi() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x000000AA;
                    r->acc.w = r->pc.w;
                    return false;

                case 1:
                    r->oReg.l = r->acc.h;
                    r->oReg.h = r->acc.l;
                    return true;

                case 2:
                    return true;

                case 3:
                    r->pc.w = 0x0066;
                    r->iff &= ~(IFF1);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
