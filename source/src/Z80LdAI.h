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

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    r->af->h = r->ir.h;
                    return false;

                case 1:
                    r->af->l &= FLAG_C;                                 // 0000000C
                    r->af->l |= (r->ir.h & (FLAG_S | FLAG_5 | FLAG_3)); // S050300C
                    r->af->l |= (r->ir.h == 0x00) ? FLAG_Z : 0x00;      // SZ50300C
                    r->af->l |= (r->iff & IFF2);                        // SZ503P0C
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
