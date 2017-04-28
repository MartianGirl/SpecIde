#pragma once

/** Z80Otdr.h
 *
 * Instruction: OTDR
 *
 * Encoding: 11 101 101  10 111 011
 * M Cycles: 5 (OCF, OCF(5), MRB, IOW, CPU(5)) if B != 00h
 *           4 (OCF, OCF(5), MRB, IOW)         if B == 00h
 * T States: 21 if B != 00h
 *           16 if B == 00h
 *
 * Flags:
 * - Let's call k = ((HL) + L)
 * - C, H are set if k > FFh
 * - P is the parity of ((k & 7h) XOR B)
 * - N is bit 7 of (HL)
 * - S, Z, 5 and 3 are set by the result of decrementing B.
 * - S, Z, 5 and 3 at the end of the instruction are 0, 1, 0, 0.
 *
 * Notes:
 * - BC register is placed on the bus before B is decremented.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Otdr : public Z80Instruction
{
    public:
        Z80Otdr() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->ioWrCycles = 1;
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x000000E2;
                    return false;

                case 1:
                    return true;

                case 2:
                    --r->hl.w;
                    r->tmp.l = r->oReg.l = r->iReg.h;
                    return true;

                case 3:
                    --r->bc.h;
                    r->af.l = r->bc.h & (FLAG_S | FLAG_5 | FLAG_3); // S.5.3...
                    r->af.l |= (r->bc.h) ? 0x00 : FLAG_Z;           // SZ5.3...
                    r->af.l |= (r->tmp.l & 0x80) >> 6;              // SZ5.3.N.
                    r->acc.w = r->tmp.l + r->hl.l;
                    r->af.l |= (r->acc.h) ? 0x00 : FLAG_H | FLAG_C; // SZ5H3.NC
                    r->acc.w = (r->acc.w & 0x07) ^ r->bc.h;
                    r->acc.l ^= r->acc.l >> 1;
                    r->acc.l ^= r->acc.l >> 2;
                    r->acc.l ^= r->acc.l >> 4;
                    r->af.l = (r->acc.l & 0x01) ? 0x00 : FLAG_PV;   // SZ5H3PNC

                    if (r->bc.h != 0x00)
                        r->cpuProcCycles = 1;
                    else
                        r->prefix = PREFIX_NO;
                    return true;

                case 4:
                case 5:
                case 6:
                case 7:
                    return false;

                case 8:
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
