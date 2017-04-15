#pragma once

/** Z80PopIy.h
 *
 * Instruction: POP IY
 *
 * Encoding: 11 111 101  11 100 001
 * M Cycles: 4 (FD, OCF, MRL, MRH)
 * T States: 15
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PopIy : public Z80Instruction
{
    public:
        Z80PopIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x000000BB;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->iy.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
