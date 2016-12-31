#pragma once

/** Z80LdRegByte.h
 *
 * Instruction: LD r, n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegByte : public Z80Instruction
{
    public:
        Z80LdRegByte() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 1;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
            r->address.w = r->pc.w;
            r->pc.w++;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->reg8[r->y]) = r->operand.h;
            r->prefix = 0;
        }
};

// vim: et:sw=4:ts=4