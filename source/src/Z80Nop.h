#pragma once

/** Z80Nop.h
 *
 * Instruction: NOP
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Nop : public Z80Instruction
{
    public:
        Z80Nop() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
