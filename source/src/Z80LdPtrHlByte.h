#pragma once

/** Z80LdPtrHlByte.h
 *
 * Instruction: LD (HL), n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrHlByte : public Z80Instruction
{
    public:
        Z80LdPtrHlByte() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 1;
            r->memWrCycles = 1;
            r->cpuWtCycles = 0;
            r->memAddrMode = 0x00000021;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
