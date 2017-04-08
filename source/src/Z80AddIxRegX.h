#pragma once

/** Z80AddIxRegX.h
 *
 * Instruction: ADD IX, rr
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AddIxRegX : public Z80Instruction
{
    public:
        Z80AddIxRegX() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    // Save HL and operand.
                    r->tmp.w = r->ix.w;
                    r->acc.w = *r->regpx[r->p];
                    return false;

                case 2:
                    // First, do the low byte addition. Carry is in lowest
                    // bit of H. Add carry here.
                    r->ix.w = r->acc.l + r->tmp.l + (r->af.l & FLAG_C);
                    r->acc.w = r->acc.h;
                    r->af.l = r->af.l & (FLAG_S | FLAG_Z | FLAG_PV);
                    r->af.l |= r->ix.h & FLAG_C;

                    // Perform the addition in H, including low byte carry.
                    r->ix.h = r->acc.l + r->tmp.h + (r->af.l & FLAG_C);
                    return false;

                case 3:
                    // Half carry
                    r->af.l |= (r->tmp.h ^ r->acc.l ^ r->ix.h) & FLAG_H;
                    return false;

                case 4:
                    // Carry
                    r->acc.w += r->tmp.h + (r->af.l & FLAG_C);
                    r->af.l &= ~FLAG_C;
                    r->af.l |= (r->acc.h & FLAG_C);
                    return false;

                case 5:
                    // 5 and 3 are affected by the high byte.
                    r->af.l |= r->ix.h & (FLAG_5 | FLAG_3);
                    return false;

                case 6:
                    return false;

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
