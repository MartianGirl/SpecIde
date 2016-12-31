#pragma once

/** Z80LdAI.h
 *
 * Instruction: LD A, I
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdAI : public Z80Instruction
{
    public:
        Z80LdAI() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 1;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->af->h = r->ir.h;
            r->af->l &= FLAG_C;                                 // 0000000C
            r->af->l |= (r->ir.h & (FLAG_S | FLAG_5 | FLAG_3)); // S050300C
            r->af->l |= (r->ir.h == 0x00) ? FLAG_Z : 0x00;      // SZ50300C
            r->af->l |= (r->iff & IFF2);                        // SZ503P0C
            r->prefix = PREFIX_NO;
        }
};

// vim: et:sw=4:ts=4
