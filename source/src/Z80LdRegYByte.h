#pragma once

/** Z80LdRegYByte.h
 *
 * Instruction: FD-prefixed LD r, n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegYByte : public Z80Instruction
{
    public:
        Z80LdRegYByte() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 1;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
            r->memAddrMode = 0x00000001;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->regy8[r->y]) = r->operand.h;
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
