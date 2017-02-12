#pragma once

/** Z80LdIxPtrWord.h
 *
 * Instruction: LD IX, (nn)
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
                    r->memWrCycles = 0;
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
