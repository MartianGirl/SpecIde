#pragma once

/** Z80LdRegYRegY.h
 *
 * Instruction: LD r, s (FD-Prefixed)
 *
 * Encoding: 11 111 101  01 rrr sss
 * M Cycles: 2 (FD, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IYh 100
 * IYl 101
 *  A  111
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegYRegY : public Z80Instruction
{
    public:
        Z80LdRegYRegY() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    *(r->regy8[r->y]) = *(r->regy8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
