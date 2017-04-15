#pragma once

/** Z80LdPtrIxReg.h
 *
 * Instruction: LD (IX+d), r
 *
 * Encoding: 11 011 101  01 110 rrr
 * M Cycles: 5 (DD, OCF, MRB, CPU, MWB)
 * T States: 19
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrIxReg : public Z80Instruction
{
    public:
        Z80LdPtrIxReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000061;
                    return true;

                case 1: // Memory read byte cycle
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
                    r->memWrCycles = 1;
                    r->oReg.l = *(r->reg8[r->z]);
                    return true;

                case 7:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
