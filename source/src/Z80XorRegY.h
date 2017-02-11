#pragma once

/** Z80XorRegY.h
 *
 * Instruction: XOR ry
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80XorRegY : public Z80Instruction
{
    public:
        Z80XorRegY() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    // Calculate the result.
                    r->acc.l = r->acc.h = r->af.h ^ *r->regy8[r->z];
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l = (r->acc.h & 0x01) ? 0x00 : FLAG_PV;   // ...0.P00
                    r->af.l |=
                        r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.503P00
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ503P00
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
