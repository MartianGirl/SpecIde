#pragma once

/** Z80LdRegXWord.h
 *
 * DD Prefixed instructions.
 * Instruction: LD BC, nn
 * Instruction: LD DE, nn
 * Instruction: LD IX, nn
 * Instruction: LD SP, nn
 *
 * Encoding: 11 011 101  00 rr0 001
 * M Cycles: 4 (DD, OCF, MRL, MRH)
 * T States: 14
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IX  10
 *  SP  11
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegXWord : public Z80Instruction
{
    public:
        Z80LdRegXWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00000011;
                    return true;

                case 1:
                    return true;

                case 2:
                    *r->regpx[r->p] = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
