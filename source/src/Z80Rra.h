#pragma once

/** Z80Rra.h
 *
 * Instruction: RRA
 *
 * Encoding: 00 011 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - LSB is copied into CF. CF is copied into MSB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rra : public Z80Instruction
{
    public:
        Z80Rra() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.l = r->af.h;
                    r->acc.h = r->af.l & FLAG_C;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_PV;
                    r->af.l |= r->af.h & FLAG_C;
                    r->acc.w >>= 1;
                    r->af.h = r->acc.l;
                    r->af.l |= r->af.h & (FLAG_5 | FLAG_3);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
