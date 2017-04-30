#pragma once

/** Z80PrefixCB.h
 *
 * Prefix CB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PrefixCB : public Z80Instruction
{
    public:
        Z80PrefixCB() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;
                    r->prefix = PREFIX_CB;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
