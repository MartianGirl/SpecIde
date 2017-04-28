#pragma once

/** Z80Ind.h
 *
 * Instruction: IND
 *
 * Encoding: 11 101 101  10 101 010
 * M Cycles: 4 (OCF, OCF(5), IOR, MWB)
 * T States: 16
 *
 * Flags:
 * - Let's call k = (HL) + ((C - 1) & FFh)
 * - C, H are set if k > FFh
 * - P is the parity of ((k & 7h) XOR B)
 * - N is bit 7 of (HL)
 * - S, Z, 5 and 3 are set by the result of decrementing B.
 *
 * Notes:
 * - BC register is placed on the bus after B is decremented.
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Ind : public Z80Instruction
{
    public:
        Z80Ind() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->ioRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x0000002E;
                    return false;

                case 1:
                    --r->bc.h;
                    return true;

                case 2:
                    r->tmp.l = r->oReg.l = r->iReg.h;
                    return true;

                case 3:
                    --r->hl.w;
                    r->af.l = r->bc.h & (FLAG_S | FLAG_5 | FLAG_3); // S.5.3...
                    r->af.l |= (r->bc.h) ? 0x00 : FLAG_Z;           // SZ5.3...
                    r->af.l |= (r->tmp.l & 0x80) >> 6;              // SZ5.3.N.
                    r->acc.w = r->tmp.l + ((r->bc.l - 1) & 0xFF);
                    r->af.l |= (r->acc.h) ? 0x00 : FLAG_H | FLAG_C; // SZ5H3.NC
                    r->acc.w = (r->acc.w & 0x07) ^ r->bc.h;
                    r->acc.l ^= r->acc.l >> 1;
                    r->acc.l ^= r->acc.l >> 2;
                    r->acc.l ^= r->acc.l >> 4;
                    r->af.l = (r->acc.l & 0x01) ? 0x00 : FLAG_PV;   // SZ5H3PNC
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
