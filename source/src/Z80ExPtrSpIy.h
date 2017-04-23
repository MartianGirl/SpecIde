#pragma once

/** Z80ExPtrSpIy.h
 *
 * Instruction: EX (SP), IY
 *
 * Encoding: 11 111 101  11 100 011
 * M Cycles: 6 (OCF, OCF, SRB(3), SRB(4), SWB(3), SWB(5))
 * T States: 23
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80ExPtrSpIy : public Z80Instruction
{
    public:
        Z80ExPtrSpIy() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x0000AABB;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->oReg.l = r->iy.h;
                    r->oReg.h = r->iy.l;
                    return false;

                case 3:
                    return true;

                case 4:
                    return true;

                case 5:
                    return false;

                case 6:
                    r->iy.w = r->iReg.w;
                    return false;

                case 7:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
