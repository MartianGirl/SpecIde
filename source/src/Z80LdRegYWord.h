#pragma once

/** Z80LdRegYWord.h
 *
 * FD Prefixed instructions.
 * Instruction: LD BC, nn
 * Instruction: LD DE, nn
 * Instruction: LD IY, nn
 * Instruction: LD SP, nn
 *
 * Encoding: 11 111 101  00 rr0 001
 * M Cycles: 4 (FD, OCF, MRL, MRH)
 * T States: 14
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

class Z80LdRegYWord : public Z80Instruction
{
    public:
        Z80LdRegYWord() {}

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
                    *r->regpy[r->p] = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
