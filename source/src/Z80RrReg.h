#pragma once

/** Z80RrReg.h
 *
 * Instruction: RR r
 *
 * Encoding: 11 001 011  00 011 rrr
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
 * - LSB is copied into CF. CF is copied into MSB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80RrReg : public Z80Instruction
{
    public:
        Z80RrReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.l = *r->reg8[r->z];
                    r->acc.h = r->af.l & FLAG_C;
                    r->af.l = r->acc.l & FLAG_C;
                    r->acc.w >>= 1;
                    *r->reg8[r->z] = r->acc.h = r->acc.l;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l |= r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);
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
