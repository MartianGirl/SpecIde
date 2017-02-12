#pragma once

/** Z80LdPtrHlReg.h
 *
 * Instruction: LD (HL), r
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrHlReg : public Z80Instruction
{
    public:
        Z80LdPtrHlReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000002;

                    r->oReg.l = *(r->reg8[r->z]);
                    return true;

                case 1:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
