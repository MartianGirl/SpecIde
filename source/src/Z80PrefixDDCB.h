#pragma once

/** Z80PrefixDDCB
 *
 * Prefix DDCB.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80PrefixDDCB : public Z80Instruction
{
    public:
        Z80PrefixDDCB() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memAddrMode = 0x00006611;
                    return true;

                case 1: // This is the displacement.
                    r->tmp.l = r->iReg.h;
                    return true;

                case 2: // This is actually the opcode. Now we *decode* it,
                        // as if we were starting a new instruction.
                        // Also, during this cycle we calculate the operand
                        // address.
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->ix.w;
                    return false;

                case 4:
                    r->opcode = r->iReg.h;
                    r->x = (r->iReg.h & 0xC0) >> 6; // xx......
                    r->y = (r->iReg.h & 0x38) >> 3; // ..yyy...
                    r->z = (r->iReg.h & 0x07);      // .....zzz
                    r->p = r->y >> 1;               // ..pp....
                    r->q = r->y & 0x01;             // ....q...

                    r->memRdCycles = 1;
                    r->memWrCycles = (r->x == 1) ? 0 : 1;
                    r->ioRdCycles = 0;
                    r->ioWrCycles = 0;
                    r->cpuProcCycles = 0;

                    r->prefix = PREFIX_DD | PREFIX_CB;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};
// vim: et:sw=4:ts=4
