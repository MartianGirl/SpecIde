#pragma once

/** Z80OutPtrCReg.h
 *
 * Instruction: OUT (C), r
 *
 * Encoding: 11 101 101  01 rrr 001
 * M Cycles: 3 (ED, OCF, IOWR)
 * T States: 12
 *
 *  Reg  r
 *  --- ---
 *   B  000
 *   C  001
 *   D  010
 *   E  011
 *   H  100
 *   L  101
 *   -  110     * Outputs 00h
 *   A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80OutPtrCReg : public Z80Instruction
{
    public:
        Z80OutPtrCReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->ioWrCycles = 1;
                    r->memAddrMode = 0x0000000E;

                    r->oReg.l = (r->y == 6) ? 0x00 : *r->reg8[r->y];
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
