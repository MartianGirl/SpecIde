#pragma once

/** Z80LdPtrIyByte.h
 *
 * Instruction: LD (IY+d), n
 *
 * Encoding: 11 111 101  00 110 110
 * M Cycles: 5 (FD, OCF, MRB, MRB/CPU, MWB)
 * T States: 19
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrIyByte : public Z80Instruction
{
    public:
        Z80LdPtrIyByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00000711;
                    return true;

                case 1:
                    r->tmp.l = r->iReg.h;
                    return true;

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->iy.w;
                    return false;

                case 4:
                    r->memWrCycles = 1;
                    r->oReg.l = r->iReg.h;
                    return true;

                case 5:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
