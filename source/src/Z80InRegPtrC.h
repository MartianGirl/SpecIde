#pragma once

/** Z80InRegPtrC.h
 *
 * Instruction: IN r, (C)
 *
 * Encoding: 11 101 101  01 rrr 000
 * M Cycles: 3 (ED, OCF, IORD)
 * T States: 12
 *
 *  Reg  r
 *  --- ---
 *   B  000
 *   C  001
 *   D  010
 *   E  011
 *   H  100
 *   L  101
 *   -  110     * Only updates the flags, drops the result.
 *   A  111
 *
 * Flags: SZ503P0*
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80InRegPtrC : public Z80Instruction
{
    public:
        Z80InRegPtrC() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->ioRdCycles = 1;
                    r->memAddrMode = 0x0000000E;
                    return true;

                case 1:
                    r->acc.h = r->acc.l = r->iReg.h;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;

                    r->af.l &= FLAG_C;
                    r->af.l |= (FLAG_S | FLAG_5 | FLAG_3) & r->acc.l;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;

                    if (r->y != 6)
                        *r->reg8[r->y] = r->iReg.h;

                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
