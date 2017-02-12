#pragma once

/** Z80Cpdr.h
 *
 * Instruction: CPDR
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Cpdr : public Z80Instruction
{
    public:
        Z80Cpdr() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000002;
                    return true;

                case 1:
                    return false;

                case 2:
                    r->bc.w--;
                    r->hl.w--;
                    return false;

                case 3:
                    r->af.l &= FLAG_C;                              // 0000000C
                    r->af.l |= FLAG_N;                              // 000000NC
                    r->acc.h = -r->iReg.h;
                    r->af.l |=                                      // 000H00NC
                        ((r->af.h & 0x0F) + (r->acc.h & 0x0F)) & FLAG_H;
                    return false;

                case 4:
                    r->acc.h += r->af.h;
                    r->af.l |= r->acc.h & FLAG_S;                   // S00H00NC
                    r->af.l |= (r->acc.h) ? 0x00 : FLAG_Z;          // SZ0H00NC
                    return false;

                case 5:
                    r->acc.h -= (r->af.l & FLAG_H) >> 4;
                    r->af.l |= (r->acc.h & FLAG_3);                 // SZ0H30NC
                    r->af.l |= (r->acc.h & FLAG_N) << 4;            // SZ5H30NC
                    r->af.l |= (r->bc.w) ? FLAG_PV : 0x00;          // SZ5H3PNC
                    return false;
                    
                case 6:
                    if (r->bc.w != 0x0000 && ((r->af.l & FLAG_Z) == 0x00))
                    {
                        return false;
                    }
                    else
                    {
                        r->prefix = PREFIX_NO;
                        return true;
                    }

                case 7:
                case 8:
                case 9:
                case 10:
                    // Interrupts are checked at the end of each transfer,
                    // so, maybe here?
                    return false;

                case 11:
                    r->pc.w -= 0x0002;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
