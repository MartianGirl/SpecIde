#pragma once

/** Z80PushIy.h
 *
 * Instruction: PUSH IY
 *
 * Encoding: 11 111 101  11 100 101
 * M Cycles: 4 (FD, OCF, MWL, MWH)
 * T States: 15
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PushIy : public Z80Instruction
{
    public:
        Z80PushIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x000000AA;
                    return false;

                case 1:
                    r->oReg.l = r->iy.h;
                    r->oReg.h = r->iy.l;
                    return true;

                case 2:
                    return true;

                case 3:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
