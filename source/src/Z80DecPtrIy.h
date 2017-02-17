#pragma once

/** Z80DecPtrIy.h
 *
 * Instruction: DEC (IY+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80DecPtrIy : public Z80Instruction
{
    public:
        Z80DecPtrIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000771;
                    return true;

                case 1:
                    r->tmp.l = r->iReg.h;
                    return false;

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->iy.w;
                    return false;

                case 4:
                    return false;

                case 5:
                    r->memRdCycles = 1;
                    return true;

                case 6:
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

                case 7:
                    r->oReg.l = r->acc.l;
                    r->memWrCycles = 1;
                    return true;

                case 8:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
