#pragma once

/** Z80Instruction
 *
 * Base class for all instruction functors.
 *
 */

#include "Z80Defs.h"
#include "Z80RegisterSet.h"

class Z80Instruction
{
    public:
        Z80Instruction() {}
        virtual bool operator()(Z80RegisterSet* r) = 0;
};

// vim: et:sw=4:ts=4
