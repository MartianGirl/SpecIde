#pragma once

/** Z80RetCc.h
 *
 * Instruction: RET NZ
 * Instruction: RET Z
 * Instruction: RET NC
 * Instruction: RET C
 * Instruction: RET PO
 * Instruction: RET PE
 * Instruction: RET P
 * Instruction: RET M
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80RetCc : public Z80Instruction
{
    public:
        Z80RetCc() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x000000BB;
                    return false;

                case 1:
                    switch (r->y)
                    {
                        case 0:
                            r->memRdCycles =
                                ((r->af.l & FLAG_Z) == 0x00) ? 2 : 0;
                            return true;

                        case 1:
                            r->memRdCycles =
                                ((r->af.l & FLAG_Z) == FLAG_Z) ? 2 : 0;
                            return true;

                        case 2: 
                            r->memRdCycles =
                                ((r->af.l & FLAG_C) == 0x00) ? 2 : 0;
                            return true;

                        case 3:
                            r->memRdCycles =
                                ((r->af.l & FLAG_C) == FLAG_C) ? 2 : 0;
                            return true;

                        case 4:
                            r->memRdCycles =
                                ((r->af.l & FLAG_PV) == 0x00) ? 2 : 0;
                            return true;

                        case 5:
                            r->memRdCycles =
                                ((r->af.l & FLAG_PV) == FLAG_PV) ? 2 : 0;
                            return true;

                        case 6:
                            r->memRdCycles =
                                ((r->af.l & FLAG_S) == 0x00) ? 2 : 0;
                            return true;

                        case 7:
                            r->memRdCycles =
                                ((r->af.l & FLAG_S) == FLAG_S) ? 2 : 0;
                            return true;

                        default:
                            assert(false);
                            return true;
                    }

                case 2:
                    return true;

                case 3:
                    r->pc.w = r->iReg.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
