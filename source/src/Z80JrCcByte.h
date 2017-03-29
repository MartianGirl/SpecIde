#pragma once

/** Z80JrCcByte.h
 *
 * JR NZ, n Instruction.
 * JR Z, n Instruction.
 * JR NC, n Instruction.
 * JR C, n Instruction.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80JrCcByte : public Z80Instruction
{
    public:
        Z80JrCcByte() {}

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
                    r->tmp.l = r->iReg.h;

                    switch (r->y)
                    {
                        case 4:
                            return ((r->af.l & FLAG_Z) == FLAG_Z);

                        case 5:
                            return ((r->af.l & FLAG_Z) == 0x00);

                        case 6:
                            return ((r->af.l & FLAG_C) == FLAG_C);

                        case 7:
                            return ((r->af.l & FLAG_C) == 0x00);

                        default:
                            assert(false);
                            return true;
                    }

                case 2:
                    r->tmp.h = ((r->tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->tmp.w += r->pc.w;
                    return false;

                case 4:
                case 5:
                    return false;

                case 6:
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
