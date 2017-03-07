#pragma once

/** Z80Daa.h
 *
 * Instruction: DAA
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Daa : public Z80Instruction
{
    public:
        Z80Daa() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    // Keep the relevant flags.
                    r->af.l &= (FLAG_H | FLAG_N | FLAG_C);          // ...H..NC
                    // Adjust the lower nybble first.
                    r->acc.w = (r->af.h & 0x0F);
                    if ((r->acc.w > 0x09) || ((r->af.l & FLAG_H) == FLAG_H))
                    {
                        if ((r->af.l & FLAG_N) == FLAG_N)   // Subtraction
                        {
                            r->af.l &= (r->acc.w > 0x05) ? ~FLAG_H : 0xFF;
                            r->acc.w -= 0x06;
                        }
                        else                                // Addition
                        {
                            r->af.l &= ~FLAG_H;
                            r->af.l |= (r->acc.w > 0x09) ? FLAG_H : 0x00;
                            r->acc.w += 0x06;
                        }
                    }

                    // Adjust the upper nybble then.
                    r->acc.w += (r->af.h & 0xF0);
                    if ((r->af.h > 0x9A) || ((r->af.l & FLAG_C) == FLAG_C))
                    {
                        if ((r->af.l & FLAG_N) == FLAG_N)   // Subtraction
                        {
                            r->acc.w -= 0x60;
                        }
                        else                                // Addition
                        {
                            r->acc.w += 0x60;
                        }

                        r->af.l |= FLAG_C;
                    }

                    r->af.l |= 
                        r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.5H3.NC
                    r->acc.h = r->acc.l;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;  // S.5H3PNC
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3PNC
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
