#pragma once

/** Z80JpHl.h
 *
 * JP (HL) Instruction.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80JpHl : public Z80Instruction
{
    public:
        Z80JpHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    
                    r->pc.w = r->hl.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
