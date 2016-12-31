#pragma once

/** Z80LdRegYRegY.h
 *
 * Instruction: LD r, r' (FD-Prefixed)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegYRegY : public Z80Instruction
{
    public:
        Z80LdRegYRegY() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
            r->memAddrMode = 0x00000000;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->regy8[r->y]) = *(r->regy8[r->z]);
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
