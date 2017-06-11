#pragma once

/** Z80AndPtrIy.h
 *
 * Instruction: AND (IY+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80AndPtrIy : public Z80Instruction
{
    public:
        Z80AndPtrIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000071;
                    return true;

                case 1:
                    r->cpuProcCycles = 1;
                    return true;

                case 2:
                    r->tmp.l = r->iReg.h;
                    return false;

                case 3:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 4:
                    r->tmp.w += r->iy.w;
                    return false;

                case 5:
                    return false;

                case 6:
                    r->memRdCycles = 1;
                    return true;

                case 7:
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
