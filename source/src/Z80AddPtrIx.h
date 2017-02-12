#pragma once

/** Z80AddPtrIx.h
 *
 * Instruction: ADD A, (IX+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AddPtrIx : public Z80Instruction
{
    public:
        Z80AddPtrIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000061;
                    return true;

                case 1:
                    r->tmp.l = r->iReg.h;
                    return false;

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->ix.w;
                    return false;

                case 4:
                    return false;

                case 5:
                    r->memRdCycles = 1;
                    return true;

                case 6:
                    // Calculate half-carry. This is done by doing a 4-bit
                    // addition. Half-carry will be in bit 4.
                    r->acc.w = (r->af.h & 0x0F) + (r->iReg.h & 0x0F);
                    r->af.l = r->acc.l & (FLAG_H | FLAG_3);         // ...H3.0.

                    // Calculate carry in the bit 7. Overflow flag is
                    // (carry in bit 7) XOR (carry in bit 8).
                    r->acc.w = (r->af.h & 0x7F) + (r->iReg.h & 0x7F);
                    r->af.l |= (r->acc.w >> 5) & FLAG_PV;

                    // Calculate the result.
                    r->acc.w = r->af.h + r->iReg.h;
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
