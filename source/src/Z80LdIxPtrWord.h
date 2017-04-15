#pragma once

/** Z80LdIxPtrWord.h
 *
 * Instruction: LD IX, (nn)
 *
 * Encoding: 11 011 101  00 101 010
 * M Cycles: 5 (DD, OCF, MRL, MRH, MRL, MRH)
 * T States: 20
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdIxPtrWord : public Z80Instruction
{
    public:
        Z80LdIxPtrWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 4;
                    r->memAddrMode = 0x00009811;
                    return true;

                case 1:
                    return true;

                case 2:
                    return true;

                case 3:
                    return true;

                case 4:
                    r->ix.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
