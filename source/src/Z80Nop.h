#pragma once

/** Z80Nop.h
 *
 * Instruction: NOP
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Nop : public Z80Instruction
{
    public:
        Z80Nop() {}

        void decode(Z80RegisterSet* r) 
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
        }
};

// vim: et:sw=4:ts=4
