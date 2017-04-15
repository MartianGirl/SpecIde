#pragma once

/** Z80PushReg.h
 *
 * Instruction: PUSH rr
 *
 * Encoding: 11 rr0 101
 * M Cycles: 3 (OCF, MWL, MWH)
 * T States: 11
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  HL  10
 *  AF  11
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PushReg : public Z80Instruction
{
    public:
        Z80PushReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x000000AA;
                    r->acc.w = *(r->regp2[r->p]);
                    return false;

                case 1:
                    r->oReg.l = r->acc.h;
                    r->oReg.h = r->acc.l;
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
