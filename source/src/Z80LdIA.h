#pragma once

/** Z80LdIA.h
 *
 * Instruction: LD I, A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdIA : public Z80Instruction
{
    public:
        Z80LdIA() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 1;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->ir.h = r->af->h;
            r->prefix = 0;
        }
};

// vim: et:sw=4:ts=4
