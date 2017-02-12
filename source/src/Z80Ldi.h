#pragma once

/** Z80Ldi.h
 *
 * Instruction: LDI
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Ldi : public Z80Instruction
{
    public:
        Z80Ldi() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000042;
                    return true;

                case 1:
                    r->oReg.l = r->iReg.h;
                    return true;

                case 2:
                    r->bc.w--;
                    r->de.w++;
                    r->hl.w++;
                    return false;

                case 3:
                    r->iReg.h += r->af.h;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_C;            // SZ.0..0C
                    r->af.l |= (r->iReg.h & FLAG_3);                // SZ.03.0C
                    r->af.l |= (r->iReg.h & FLAG_N) << 4;           // SZ503.0C
                    r->af.l |= (r->bc.w) ? FLAG_PV : 0x00;          // SZ503P0C
                    return false;

                case 4:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
