#pragma once

/** Z80Ccf.h
 *
 * Instruction: CCF
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Ccf : public Z80Instruction
{
    public:
        Z80Ccf() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    r->af.l &= (FLAG_S | FLAG_Z | FLAG_PV | FLAG_C);
                    r->af.l |= r->af.h & (FLAG_5 | FLAG_3);
                    r->af.l |= ((r->af.l & FLAG_C) << 4) & FLAG_H;
                    r->af.l ^= FLAG_C;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
