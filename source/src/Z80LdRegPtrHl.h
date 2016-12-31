#pragma once

/** Z80LdRegPtrHl.h
 *
 * Instruction: LD r, (HL)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegPtrHl : public Z80Instruction
{
    public:
        Z80LdRegPtrHl() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 1;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
            r->memAddrMode = 0x00000002;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->reg8[r->y]) = r->operand.h;
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
