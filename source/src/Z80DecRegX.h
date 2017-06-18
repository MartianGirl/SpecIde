#pragma once

/** Z80DecRegX.h
 *
 * Instruction: DEC rx
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80DecRegX : public Z80Instruction
{
    public:
        Z80DecRegX() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->tmp.l = *r->regx8[r->y];
                    r->acc.w = r->tmp.l - 1;

                    // Preserve carry flag.
                    r->af.l &= FLAG_C;
                    r->af.l |= FLAG_N;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l ^ r->tmp.l) & FLAG_H;
                    r->af.l |= (((r->acc.l ^ r->tmp.l) >> 5)
                            ^ (r->acc.h << 2)) & FLAG_PV;
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
                    *r->regx8[r->y] = r->acc.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
