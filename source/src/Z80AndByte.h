#pragma once

/** Z80AndByte.h
 *
 * Instruction: AND n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AndByte : public Z80Instruction
{
    public:
        Z80AndByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000001;
                    return true;

                case 1:
                    // Calculate the result.
                    r->acc.l = r->acc.h = r->af.h & r->iReg.h;
                    r->acc.h ^= r->acc.h >> 1;
                    r->acc.h ^= r->acc.h >> 2;
                    r->acc.h ^= r->acc.h >> 4;
                    r->af.l = (r->acc.h & 0x01) 
                        ? FLAG_H : FLAG_H | FLAG_PV;                // ...H.P00
                    r->af.l |=
                        r->acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.5H3P00
                    r->af.l |= (r->acc.l) ? 0x00 : FLAG_Z;          // SZ5H3P00
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
