#pragma once

/** Z80JpCcWord.h
 *
 * JP NZ, nn Instruction.
 * JP Z, nn Instruction.
 * JP NC, nn Instruction.
 * JP C, nn Instruction.
 * JP PO, nn Instruction.
 * JP PE, nn Instruction.
 * JP P, nn Instruction.
 * JP M, nn Instruction.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80JpCcWord : public Z80Instruction
{
    public:
        Z80JpCcWord() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000011;
                    return true;

                case 1:
                    return true;

                case 2:
                    switch (r->y)
                    {
                        case 0:
                            if ((r->af.l & FLAG_Z) == 0x00)
                                r->pc.w = r->iReg.w;
                            break;
                        case 1:
                            if ((r->af.l & FLAG_Z) == FLAG_Z)
                                r->pc.w = r->iReg.w;
                            break;
                        case 2:
                            if ((r->af.l & FLAG_C) == 0x00)
                                r->pc.w = r->iReg.w;
                            break;
                        case 3:
                            if ((r->af.l & FLAG_C) == FLAG_C)
                                r->pc.w = r->iReg.w;
                            break;
                        case 4:
                            if ((r->af.l & FLAG_PV) == 0x00)
                                r->pc.w = r->iReg.w;
                            break;
                        case 5: 
                            if ((r->af.l & FLAG_PV) == FLAG_PV)
                                r->pc.w = r->iReg.w;
                            break;
                        case 6:
                            if ((r->af.l & FLAG_S) == 0x00)
                                r->pc.w = r->iReg.w;
                            break;
                        case 7:
                            if ((r->af.l & FLAG_S) == FLAG_S)
                                r->pc.w = r->iReg.w;
                            break;
                        default: assert(false);
                    }
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
