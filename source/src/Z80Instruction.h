#pragma once

/** Z80Instruction
 *
 * Base class for all instruction functors.
 *
 */

#include "Z80RegisterSet.h"

class Z80Instruction
{
    public:
        Z80Instruction() {}
        virtual void decode(Z80RegisterSet* r) {}
        virtual void operator()(Z80RegisterSet* r) {}
};

// vim: et:sw=4:ts=4
