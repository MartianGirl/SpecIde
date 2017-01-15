#pragma once

/** Z80PushIy.h
 *
 * Instruction: PUSH IY
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
                    r->memRdCycles = 0;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x000000AA;
                    r->operand.w = r->iy.w;
                    return false;

                case 1:
                    r->outWord.l = r->operand.h;
                    r->outWord.h = r->operand.l;
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