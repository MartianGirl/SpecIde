#pragma once

/** Z80LdPtrHlByte.h
 *
 * Instruction: LD (HL), n
 *
 * Encoding: 00 110 110
 * M Cycles: 3 (OCF, MRB, MWB)
 * T States: 10
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrHlByte : public Z80Instruction
{
    public:
        Z80LdPtrHlByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000021;
                    return true;

                case 1:
                    r->oReg.l = r->iReg.h;
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
