#pragma once

/** Z80OutPtrByteA.h
 *
 * Instruction: OUT (n), A
 *
 * Encoding: 11 010 011
 * M Cycles: 3 (OCF, MRB, IOWR)
 * T States: 11
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80OutPtrByteA : public Z80Instruction
{
    public:
        Z80OutPtrByteA() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->ioWrCycles = 1;
                    r->memAddrMode = 0x000000D1;
                    return true;

                case 1:
                    r->oReg.l = r->af.h;
                    return true;

                case 2:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
