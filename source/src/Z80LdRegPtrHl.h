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

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000002;
                    return true;

                case 1:
                    *(r->reg8[r->y]) = r->operand.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
