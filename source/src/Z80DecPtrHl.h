#pragma once

/** Z80DecPtrHl.h
 *
 * Instruction: DEC (HL)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80DecPtrHl : public Z80Instruction
{
    public:
        Z80DecPtrHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000022;
                    return true;

                case 1:
                    // Preserve carry bit.
                    r->af.l &= FLAG_C;

                    // Calculate half-carry. This is done by doing a 4-bit
                    // subtraction. Half-carry will be in bit 4.
                    r->acc.w = (r->iReg.h & 0x0F) - 1;
                    r->af.l |= 
                        (r->acc.w & (FLAG_H | FLAG_3)) | FLAG_N;    // ...H3.1.  

                    // Calculate carry in the bit 7. Overflow flag is
                    // (carry in bit 7) XOR (carry in bit 8).
                    r->acc.w = (r->iReg.h & 0x7F) - 1;
                    r->af.l |= (r->acc.w >> 5) & FLAG_PV;

                    // Calculate the result.
                    r->acc.w = r->iReg.h - 1;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5);        // S.5H3.1.
                    r->af.l ^= (r->acc.w >> 6) & FLAG_PV;           // S.5H3V1.
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V1.
                    return false;

                case 2:
                    r->oReg.l = r->acc.l;
                    r->memWrCycles = 1;
                    return true;

                case 3:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
