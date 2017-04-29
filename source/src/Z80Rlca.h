#pragma once

/** Z80Rlca.h
 *
 * Instruction: RLCA
 *
 * Encoding: 00 000 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - MSB is copied into LSB and into CF.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rlca : public Z80Instruction
{
    public:
        Z80Rlca() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.w = r->af.h << 1;
                    r->af.h = r->acc.l | r->acc.h;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_PV;
                    r->af.l |= r->af.h & (FLAG_5 | FLAG_3 | FLAG_C);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
