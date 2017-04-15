#pragma once

/** Z80LdIA.h
 *
 * Instruction: LD I, A
 *
 * Encoding: 11 101 101  01 000 111
 * M Cycles: 2 (ED, OCF)
 * T States: 9
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
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    r->ir.h = r->af.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;

            }
        }
};

// vim: et:sw=4:ts=4
