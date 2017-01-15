#pragma once

/** Z80Exx.h
 *
 * Instruction: EXX
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Exx : public Z80Instruction
{
    public:
        Z80Exx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    r->operand.w = r->bc.w;
                    r->bc.w = r->bc_.w;
                    r->bc_.w = r->operand.w;

                    r->operand.w = r->de.w;
                    r->de.w = r->de_.w;
                    r->de_.w = r->operand.w;

                    r->operand.w = r->hl.w;
                    r->hl.w = r->hl_.w;
                    r->hl_.w = r->operand.w;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
