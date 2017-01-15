#pragma once

/** Z80LdIA.h
 *
 * Instruction: LD I, A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdIA : public Z80Instruction
{
    public:
        Z80LdIA() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    r->ir.h = r->af.h;
                    return false;

                case 1:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;

            }
        }
};

// vim: et:sw=4:ts=4
