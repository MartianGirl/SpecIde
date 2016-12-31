#pragma once

/** Z80LdRA.h
 *
 * Instruction: LD R, A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRA : public Z80Instruction
{
    public:
        Z80LdRA() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 1;
            r->memAddrMode = 0x00000000;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->ir.l = r->af->h;
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
