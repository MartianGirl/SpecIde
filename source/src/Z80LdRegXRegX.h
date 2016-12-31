#pragma once

/** Z80LdRegXRegX.h
 *
 * Instruction: LD r, r' (DD-Prefixed)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegXRegX : public Z80Instruction
{
    public:
        Z80LdRegXRegX() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->regx8[r->y]) = *(r->regx8[r->z]);
            r->prefix = 0;
        }
};

// vim: et:sw=4:ts=4
