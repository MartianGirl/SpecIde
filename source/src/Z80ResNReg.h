#pragma once

/** Z80ResNReg.h
 *
 * Instruction: RES n, r
 *
 * Encoding: 11 001 011  10 bbb rrr
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

class Z80ResNReg : public Z80Instruction
{
    public:
        Z80ResNReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    *r->reg8[r->z] &= ~(1 << r->y);

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
