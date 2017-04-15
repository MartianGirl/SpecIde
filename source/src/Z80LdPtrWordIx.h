#pragma once

/** Z80LdPtrWordIx.h
 *
 * Instruction: LD (nn), IX
 *
 * Encoding: 11 011 101  00 100 010
 * M Cycles: 6 (DD, OCF, MRL, MRH, MWL, MWH)
 * T States: 20
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrWordIx : public Z80Instruction
{
    public:
        Z80LdPtrWordIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00009811;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->memWrCycles = 2;
                    r->oReg.w = r->ix.w;
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
