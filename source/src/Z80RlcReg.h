#pragma once

/** Z80RlcReg.h
 *
 * Instruction: RLC r
 *
 * Encoding: 11 001 011  00 000 rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 * Flags: SZ503P0C
 * - MSB is copied into LSB and into CF.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80RlcReg : public Z80Instruction
{
    public:
        Z80RlcReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.w = *r->reg8[r->z] << 1;
                    r->acc.h = r->acc.l = r->acc.l | r->acc.h;
                    *r->reg8[r->z] = r->acc.l;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l = r->acc.l & (FLAG_S | FLAG_5 | FLAG_3 | FLAG_C);
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;
                    r->af.l |= (r->acc.h & 0x01) ? 0x00 : FLAG_PV;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
