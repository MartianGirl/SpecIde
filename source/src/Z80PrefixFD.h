#pragma once

/** Z80PrefixFD
 *
 * Prefix FD.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"
#include "Z80Defs.h"

class Z80PrefixFD : public Z80Instruction
{
    public:
        Z80PrefixFD() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
            r->cpuWtCycles = 0;
        }

        void operator()(Z80RegisterSet* r)
        {
            r->prefix = PREFIX_FD;
        }
};

// vim: et:sw=4:ts=4
