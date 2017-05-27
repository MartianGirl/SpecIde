#pragma once

/** Z80SbcHlReg.h
 *
 * Instruction: SBC HL, rr
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80SbcHlReg : public Z80Instruction
{
    public:
        Z80SbcHlReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;
                    return false;

                case 1:
                    // Save HL and operand.
                    r->tmp.w = r->hl.w;
                    r->acc.w = *r->regp[r->p];
                    return false;

                case 2:
                    // First, do the low byte subtraction. Carry is in lowest
                    // bit of H. Add carry here.
                    r->hl.w = r->tmp.l - r->acc.l - (r->af.l & FLAG_C);
                    r->acc.w = r->acc.h;
                    r->af.l = r->hl.h & FLAG_C;

                    // Perform the subtraction in H, including low byte carry.
                    r->hl.h = r->tmp.h - r->acc.l - (r->af.l & FLAG_C);
                    return false;

                case 3:
                    // Now check flags:
                    // Half carry
                    r->af.l |= (r->tmp.h ^ r->acc.l ^ r->hl.h) & FLAG_H;

                    // Carry into bit 7
                    r->af.l |= ((r->tmp.h ^ r->acc.l ^ r->hl.h) >> 5) & FLAG_PV;
                    return false;

                case 4:
                    // Carry out of bit 7
                    r->acc.w = r->tmp.h - r->acc.l - (r->af.l & FLAG_C);
                    r->af.l ^= ((r->acc.h & FLAG_C) << 2) & FLAG_PV;
                    r->af.l &= ~FLAG_C;
                    r->af.l |= (r->acc.h & FLAG_C);
                    return false;

                case 5:
                    // Sign is affected by the 16-bit result - hence high byte.
                    // 5 and 3 are affected by the high byte.
                    r->af.l |= r->hl.h & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= FLAG_N;
                    return false;

                case 6:
                    // Zero is affected by the 16-bit result.
                    r->af.l |= (r->hl.w == 0x0000) ? FLAG_Z : 0x00;
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
