#pragma once

/** Z80LdPtrWordReg.h
 *
 * Instruction: LD (nn), rr
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrWordReg : public Z80Instruction
{
    public:
        Z80LdPtrWordReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00009811;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->memWrCycles = 2;
                    r->outWord.w = *(r->regp[r->p]);
                    return true;

                case 3:
                    return true;

                case 4:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
