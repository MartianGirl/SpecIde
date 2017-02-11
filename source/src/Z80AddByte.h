#pragma once

/** Z80AddByte.h
 *
 * Instruction: ADD A, n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AddByte : public Z80Instruction
{
    public:
        Z80AddByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000001;
                    return true;

                case 1:
                    // Calculate half-carry. This is done by doing a 4-bit
                    // addition. Half-carry will be in bit 4.
                    r->acc.w = (r->af.h & 0x0F) + (r->operand.h & 0x0F);
                    r->af.l = r->acc.l & (FLAG_H | FLAG_3);         // ...H3.0.

                    // Calculate carry in the bit 7. Overflow flag is
                    // (carry in bit 7) XOR (carry in bit 8).
                    r->acc.w = (r->af.h & 0x7F) + (r->operand.h & 0x7F);
                    r->af.l |= (r->acc.w >> 5) & FLAG_PV;

                    // Calculate the result.
                    r->acc.w = r->af.h + r->operand.h;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5);        // S.5H3.0.
                    r->af.l |= r->acc.h & FLAG_C;                   // S.5H3.0C
                    r->af.l ^= (r->acc.w >> 6) & FLAG_PV;           // S.5H3V0C
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
                    r->af.h = r->acc.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
