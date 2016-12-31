#pragma once

/** Z80PrefixED
 *
 * Prefix ED.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"
#include "Z80Defs.h"

class Z80PrefixED : public Z80Instruction
{
    public:
        Z80PrefixED() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->prefix = PREFIX_ED;
        }
};

// vim: et:sw=4:ts=4

