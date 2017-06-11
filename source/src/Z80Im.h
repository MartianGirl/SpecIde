#pragma once

/** Z80Im.h
 *
 * Instruction: IM 0
 * Instruction: IM 1
 * Instruction: IM 2
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Im : public Z80Instruction
{
    public:
        Z80Im() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    if ((r->y & 0x02) == 0x00)
                        r->im = 0;
                    else if ((r->y & 0x01) == 0x00)
                        r->im = 1;
                    else
                        r->im = 2;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
