#pragma once

/** Z80LdSpHl.h
 *
 * Instruction: LD SP, HL
 *
 * Encoding: 11 111 001
 * M Cycles: 1 (OCF)
 * T States: 6
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdSpHl : public Z80Instruction
{
    public:
        Z80LdSpHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    r->sp.w = r->hl.w;
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
