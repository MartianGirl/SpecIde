#pragma once

/** Z80Rla.h
 *
 * Instruction: RLA
 *
 * Encoding: 00 010 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - MSB is copied into CF. CF is copied into LSB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rla : public Z80Instruction
{
    public:
        Z80Rla() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.w = r->af.h << 1;
                    r->af.h = r->acc.l | (r->af.l & FLAG_C);
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_PV;
                    r->af.l |= r->af.h & (FLAG_5 | FLAG_3);
                    r->af.l |= r->acc.h & FLAG_C;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
