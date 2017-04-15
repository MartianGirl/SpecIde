#pragma once

/** Z80PopReg.h
 *
 * Instruction: POP rr
 *
 * Encoding: 11 rr0 001
 * M Cycles: 3 (OCF, MRL, MRH)
 * T States: 10
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

class Z80PopReg : public Z80Instruction
{
    public:
        Z80PopReg() {}

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
                    *(r->regp2[r->p]) = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
