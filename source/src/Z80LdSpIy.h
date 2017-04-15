#pragma once

/** Z80LdSpIy.h
 *
 * Instruction: LD SP, IY
 *
 * Encoding: 11 111 101  11 111 001
 * M Cycles: 2 (FD, OCF)
 * T States: 10
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdSpIy : public Z80Instruction
{
    public:
        Z80LdSpIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    r->sp.w = r->iy.w;
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
