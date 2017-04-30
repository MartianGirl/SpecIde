#pragma once

/** Z80ResNPtrIxIy.h
 *
 * Instruction: RES n, (IX + d)
 * Instruction: RES n, (IX + d), r
 * Instruction: RES n, (IY + d)
 * Instruction: RES n, (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  10 bbb rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  10 bbb rrr
 * M Cycles: 6 (OCF, OCF, MRB(5), MRB, MRB(4), MWB)
 * T States: 23
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  -  110
 *  A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80ResNPtrIxIy : public Z80Instruction
{
    public:
        Z80ResNPtrIxIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 5:
                    r->acc.l = r->iReg.h;
                    r->acc.l &= ~(1 << r->y);
                    if (r->z != 6)
                        *r->reg8[r->z] = r->acc.l;
                    return false;

                case 6:
                    r->oReg.l = r->acc.l;
                    return true;

                case 7:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
