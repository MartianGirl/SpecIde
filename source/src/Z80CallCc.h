#pragma once

/** Z80CallCc.h
 *
 * Instruction: CALL NC, nn
 * Instruction: CALL C, nn
 * Instruction: CALL NZ, nn
 * Instruction: CALL Z, nn
 * Instruction: CALL PO, nn
 * Instruction: CALL PE, nn
 * Instruction: CALL P, nn
 * Instruction: CALL M, nn
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80CallCc : public Z80Instruction
{
    public:
        Z80CallCc() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x0000AA11;
                    return true;

                case 1:
                    return true;

                case 2:
                    switch (r->y)
                    {
                        case 0: return ((r->af.l & FLAG_Z) == FLAG_Z);
                        case 1: return ((r->af.l & FLAG_Z) == 0x00);
                        case 2: return ((r->af.l & FLAG_C) == FLAG_C);
                        case 3: return ((r->af.l & FLAG_C) == 0x00);
                        case 4: return ((r->af.l & FLAG_PV) == FLAG_PV);
                        case 5: return ((r->af.l & FLAG_PV) == 0x00);
                        case 6: return ((r->af.l & FLAG_S) == FLAG_S);
                        case 7: return ((r->af.l & FLAG_S) == 0x00);
                        default: assert(false); return true;
                    }

                case 3:
                    r->oReg.l = r->pc.h;
                    r->oReg.h = r->pc.l;
                    r->memWrCycles = 2;
                    return true;

                case 4:
                    return true;

                case 5:
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
