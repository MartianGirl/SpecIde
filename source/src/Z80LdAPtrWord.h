#pragma once

/** Z80LdAPtrWord.h
 *
 * Instruction: LD A, (nn)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdAPtrWord : public Z80Instruction
{
    public:
        Z80LdAPtrWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 3;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000811;
                    return true;

                case 1:
                    return true;

                case 2:
                    return true;

                case 3:
                    r->af->h = r->operand.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4

