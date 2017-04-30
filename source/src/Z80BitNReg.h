#pragma once

/** Z80BitNReg.h
 *
 * Instruction: BIT n, r
 *
 * Encoding: 11 001 011  01 bbb rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80BitNReg : public Z80Instruction
{
    public:
        Z80BitNReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.l = *r->reg8[r->z];
                    r->acc.l &= (1 << r->y);

                    r->af.l &= FLAG_C;
                    r->af.l |= FLAG_H;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z | FLAG_PV;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
