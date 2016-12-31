#pragma once

/** Z80LdRegXByte.h
 *
 * Instruction: DD-prefixed LD r, n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegXByte : public Z80Instruction
{
    public:
        Z80LdRegXByte() {}

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
            *(r->regx8[r->y]) = r->operand.h;
            r->prefix = 0;
        }
};

// vim: et:sw=4:ts=4
