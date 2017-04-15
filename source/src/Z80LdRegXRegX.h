#pragma once

/** Z80LdRegXRegX.h
 *
 * Instruction: LD r, s (DD-Prefixed)
 *
 * Encoding: 11 011 101  01 rrr sss
 * M Cycles: 2 (DD, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IXh 100
 * IXl 101
 *  A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegXRegX : public Z80Instruction
{
    public:
        Z80LdRegXRegX() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    *(r->regx8[r->y]) = *(r->regx8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
