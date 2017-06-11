#pragma once

/** Z80BitNPtrIxIy.h
 *
 * Instruction: BIT n, (IX + d)
 * Instruction: BIT n, (IY + d)
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  01 bbb rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  01 bbb rrr
 * M Cycles: 2 (OCF, OCF, MRB(5), MRB, MRB(4))
 * T States: 20
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80BitNPtrIxIy : public Z80Instruction
{
    public:
        Z80BitNPtrIxIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 5:
                    r->acc.l = r->iReg.h;
                    r->acc.l &= (1 << r->y);
                    return false;

                case 6:
                    r->af.l &= FLAG_C;
                    r->af.l |= FLAG_H;
                    r->af.l |= r->acc.l & FLAG_S;
                    r->af.l |= r->tmp.h & (FLAG_5 | FLAG_3);
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
