#pragma once

/** Z80PrefixDD
 *
 * Prefix DD.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PrefixDD : public Z80Instruction
{
    public:
        Z80PrefixDD() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    r->prefix = PREFIX_DD;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
