#pragma once

/** Z80LdRegReg.h
 *
 * Instruction: LD r, r'
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegReg : public Z80Instruction
{
    public:
        Z80LdRegReg() {}

        void decode(Z80RegisterSet* r)
        {
            r->memRdCycles = 0;
            r->memWrCycles = 0;
        }

        void operator()(Z80RegisterSet* r)
        {
            *(r->reg8[r->y]) = *(r->reg8[r->z]);
        }
};

// vim: et:sw=4:ts=4
