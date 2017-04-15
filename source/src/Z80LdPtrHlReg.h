#pragma once

/** Z80LdPtrHlReg.h
 *
 * Instruction: LD (HL), r
 *
 * Encoding: 01 110 rrr
 * M Cycles: 2 (OCF, MWB)
 * T States: 7
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

class Z80LdPtrHlReg : public Z80Instruction
{
    public:
        Z80LdPtrHlReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000002;

                    r->oReg.l = *(r->reg8[r->z]);
                    return true;

                case 1:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
