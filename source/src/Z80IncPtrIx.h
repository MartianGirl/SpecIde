#pragma once

/** Z80IncPtrIx.h
 *
 * Instruction: INC (IX+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80IncPtrIx : public Z80Instruction
{
    public:
        Z80IncPtrIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00000661;
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
                    r->acc.w = r->iReg.h + 1;

                    // Preserve carry flag.
                    r->af.l &= FLAG_C;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
                    r->af.l |= (r->acc.l ^ r->iReg.h) & FLAG_H;
                    r->af.l |= (((r->acc.l ^ r->iReg.h) >> 5) 
                            ^ (r->acc.h << 2)) & FLAG_PV;
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
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
