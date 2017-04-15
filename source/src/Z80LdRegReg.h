#pragma once

/** Z80LdRegReg.h
 *
 * Instruction: LD r, s
 *
 * Encoding: 01 rrr sss
 * M Cycles: 1 (OCF)
 * T States: 4
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

class Z80LdRegReg : public Z80Instruction
{
    public:
        Z80LdRegReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    *(r->reg8[r->y]) = *(r->reg8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
