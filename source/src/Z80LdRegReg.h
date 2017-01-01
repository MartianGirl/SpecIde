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

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    *(r->reg8[r->y]) = *(r->reg8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
