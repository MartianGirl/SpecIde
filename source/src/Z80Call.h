#pragma once

/** Z80Call.h
 *
 * Instruction: CALL nn
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Call : public Z80Instruction
{
    public:
        Z80Call() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x0000AA11;
                    return true;

                case 1:
                    return true;

                case 2:
                    return false;

                case 3:
                    r->oReg.l = r->pc.h;
                    r->oReg.h = r->pc.l;
                    return true;

                case 4:
                    return true;

                case 5:
                    r->pc.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
