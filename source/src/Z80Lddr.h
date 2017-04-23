#pragma once

/** Z80Lddr.h
 *
 * Instruction: LDDR
 *
 * Encoding: 11 101 101  10 111 000
 * M Cycles: 5 (OCF, OCF, MRB, MWB(5), CPU(5)) if BC != 0000h
 *           4 (OCF, OCF, MRB, MWB(5))         if BC == 0000h
 * T States: 21 if BC != 0000h
 *           16 if BC == 0000h
 *
 * Flags:
 * - Let's call 'n' the value of the byte being transferred plus the value
 *   of register A.
 * - S, Z, C are not changed.
 * - H is always zero.
 * - P/V is set if BC != 0000h
 * - 3 is bit 3 of n.
 * - 5 is bit 1 of n.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Lddr : public Z80Instruction
{
    public:
        Z80Lddr() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000042;
                    return true;

                case 1:
                    r->oReg.l = r->iReg.h;
                    return true;

                case 2:
                    --r->bc.w;
                    --r->de.w;
                    --r->hl.w;
                    return false;

                case 3:
                    r->tmp.l = r->iReg.h + r->af.h;
                    r->af.l &= FLAG_S | FLAG_Z | FLAG_C;            // SZ00000C
                    r->af.l |= (r->tmp.l & FLAG_3);                 // SZ00300C
                    r->af.l |= (r->tmp.l << 4) & FLAG_5;            // SZ50300C
                    r->af.l |= (r->bc.w) ? FLAG_PV : 0x00;          // SZ503P0C
                    return false;

                case 4:
                    if (r->bc.w != 0x0000)
                        r->cpuProcCycles = 1;
                    else
                        r->prefix = PREFIX_NO;
                    return true;

                case 5:
                case 6:
                case 7:
                case 8:
                    return false;

                case 9:
                    r->pc.w -= 0x0002;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
