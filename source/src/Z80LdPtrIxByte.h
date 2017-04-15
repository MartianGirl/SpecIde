#pragma once

/** Z80LdPtrIxByte.h
 *
 * Instruction: LD (IX+d), n
 *
 * Encoding: 11 011 101  00 110 110
 * M Cycles: 5 (DD, OCF, MRB, MRB/CPU, MWB)
 * T States: 19
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrIxByte : public Z80Instruction
{
    public:
        Z80LdPtrIxByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00000611;
                    return true;

                case 1:
                    r->tmp.l = r->iReg.h;
                    return true;

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->ix.w;
                    return false;

                case 4:
                    r->memWrCycles = 1;
                    r->oReg.l = r->iReg.h;
                    return true;

                case 5:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
