#pragma once

/** Z80CpPtrIx.h
 *
 * Instruction: CP (IX+d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80CpPtrIx : public Z80Instruction
{
    public:
        Z80CpPtrIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000061;
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
                    r->tmp.w += r->ix.w;
                    return false;

                case 5:
                    return false;

                case 6:
                    r->memRdCycles = 1;
                    return true;

                case 7:
                    r->tmp.l = r->iReg.h;
                    r->acc.w = r->af.h - r->tmp.l;

                    // Flags 5 & 3 are copied from the operand.
                    r->af.l = r->tmp.l & (FLAG_5 | FLAG_3);
                    r->af.l |= r->acc.l & FLAG_S;
                    r->af.l |= r->acc.h & FLAG_C;
                    r->af.l |= FLAG_N;
                    r->af.l |= (r->acc.l ^ r->af.h ^ r->tmp.l) & FLAG_H;
                    r->af.l |= (((r->acc.l ^ r->tmp.l ^ r->af.h) >> 5) 
                            ^ (r->acc.h << 2)) & FLAG_PV;
                    r->af.l |= r->acc.l ? 0x00 : FLAG_Z;            // SZ5H3VNC
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
