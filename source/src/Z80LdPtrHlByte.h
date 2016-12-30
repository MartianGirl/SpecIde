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
            r->address.w = r->pc.w;
            r->pc.w++;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->address.w = r->hl->w;
            r->memWrCycles--;
        }
};

// vim: et:sw=4:ts=4
