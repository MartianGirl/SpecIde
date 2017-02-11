#pragma once

/** Z80DecRegY.h
 *
 * Instruction: DEC ry
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80DecRegY : public Z80Instruction
{
    public:
        Z80DecRegY() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    // Preserve carry bit.
                    r->af.l &= FLAG_C;

                    // Calculate half-carry. This is done by doing a 4-bit
                    // subtraction. Half-carry will be in bit 4.
                    r->acc.w = (*r->regy8[r->y] & 0x0F) - 1;
                    r->af.l |= 
                        (r->acc.w & (FLAG_H | FLAG_3)) | FLAG_N;    // ...H3.1.  

                    // Calculate carry in the bit 7. Overflow flag is
                    // (carry in bit 7) XOR (carry in bit 8).
                    r->acc.w = (*r->regy8[r->y] & 0x7F) - 1;
                    r->af.l |= (r->acc.w >> 5) & FLAG_PV;

                    // Calculate the result.
                    r->acc.w = *r->regy8[r->y] - 1;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5);        // S.5H3.1.
                    r->af.l ^= (r->acc.w >> 6) & FLAG_PV;           // S.5H3V1.
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V1.
                    *r->regy8[r->y] = r->acc.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
