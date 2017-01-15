#pragma once

/** Z80Ldir.h
 *
 * Instruction: LDIR
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Ldir : public Z80Instruction
{
    public:
        Z80Ldir() {}

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
                    r->outWord.l = r->operand.h;
                    return true;

                case 2:
                    r->bc.w--;
                    r->de.w++;
                    r->hl.w++;
                    return false;

                case 3:
                    r->operand.h += r->af.h;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_C;            // SZ00000C
                    r->af.l |= (r->operand.h & FLAG_3);             // SZ00300C
                    r->af.l |= (r->operand.h & FLAG_N) << 4;        // SZ50300C
                    r->af.l |= (r->bc.w) ? FLAG_PV : 0x00;          // SZ503P0C
                    return false;

                case 4:
                    if (r->bc.w)
                    {
                        return false;
                    }
                    else
                    {
                        r->prefix = PREFIX_NO;
                        return true;
                    }

                case 5:
                case 6:
                case 7:
                case 8:
                    // Interrupts are checked at the end of each transfer,
                    // so, maybe here?
                    return false;

                case 9:
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
