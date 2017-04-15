#pragma once

/** Z80LdRegPtrWord.h
 *
 * Instruction: LD rr, (nn)
 *
 * Encoding: 11 101 101  01 rr1 011
 * M Cycles: 5 (ED, OCF, MRL, MRH, MRL, MRH)
 * T States: 20
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IY  10
 *  SP  11
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegPtrWord : public Z80Instruction
{
    public:
        Z80LdRegPtrWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 4;
                    r->memAddrMode = 0x00009811;
                    return true;

                case 1:
                    return true;

                case 2:
                    return true;

                case 3:
                    return true;

                case 4:
                    *(r->regp[r->p]) = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
