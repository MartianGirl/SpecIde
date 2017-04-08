#pragma once

/** Z80DecRegPX.h
 *
 * Instruction: DEC rx
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80DecRegPX : public Z80Instruction
{
    public:
        Z80DecRegPX() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    // Decrement operand.
                    --*r->regpx[r->p];
                    return false;

                case 2:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
