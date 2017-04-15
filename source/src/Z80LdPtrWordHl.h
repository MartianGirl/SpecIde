#pragma once

/** Z80LdPtrWordHl.h
 *
 * Instruction: LD (nn), HL
 *
 * Encoding: 00 100 010
 * M Cycles: 5 (OCF, MRL, MRH, MWL, MWH)
 * T States: 16
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrWordHl : public Z80Instruction
{
    public:
        Z80LdPtrWordHl() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00009811;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->memWrCycles = 2;
                    r->oReg.w = r->hl.w;
                    return true;

                case 3:
                    return true;

                case 4:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
