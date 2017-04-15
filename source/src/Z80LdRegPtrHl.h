#pragma once

/** Z80LdRegPtrHl.h
 *
 * Instruction: LD r, (HL)
 *
 * Encoding: 01 rrr 110
 * M Cycles: 2 (OCF, MRB)
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

class Z80LdRegPtrHl : public Z80Instruction
{
    public:
        Z80LdRegPtrHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000002;
                    return true;

                case 1:
                    *(r->reg8[r->y]) = r->iReg.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
