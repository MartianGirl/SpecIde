#pragma once

/** Z80Exx.h
 *
 * Instruction: EXX
 *
 * Encoding: 11 011 001
 * M Cycles: 1 (OCF)
 * T States: 4
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
                    r->memAddrMode = 0x00000000;

                    r->acc.w = r->bc.w;
                    r->bc.w = r->bc_.w;
                    r->bc_.w = r->acc.w;

                    r->acc.w = r->de.w;
                    r->de.w = r->de_.w;
                    r->de_.w = r->acc.w;

                    r->acc.w = r->hl.w;
                    r->hl.w = r->hl_.w;
                    r->hl_.w = r->acc.w;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
