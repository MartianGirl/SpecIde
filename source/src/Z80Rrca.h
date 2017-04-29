#pragma once

/** Z80Rrca.h
 *
 * Instruction: RRCA
 *
 * Encoding: 00 001 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - LSB is copied into MSB and into CF.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rrca : public Z80Instruction
{
    public:
        Z80Rrca() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.h = r->af.h & 0x01;
                    r->acc.l = r->af.h;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_PV;
                    r->af.l |= r->acc.h & FLAG_C;
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
