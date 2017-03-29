#pragma once

/** Z80Djnz.h
 *
 * DJNZ n Instruction.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Djnz : public Z80Instruction
{
    public:
        Z80Djnz() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000001;
                    return true;

                case 1:
                    --r->bc.h;
                    return false;

                case 2:
                    return (r->bc.h == 0x00);

                case 3:
                    r->tmp.l = r->iReg.h;
                    return false;

                case 4:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 5:
                    r->tmp.w += r->pc.w;
                    return false;

                case 6:
                    return false;

                case 7:
                    r->pc.w = r->tmp.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
