#pragma once

/** Z80LdIyWord.h
 *
 * Instruction: LD IY, nn
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdIyWord : public Z80Instruction
{
    public:
        Z80LdIyWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000011;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->iy.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
