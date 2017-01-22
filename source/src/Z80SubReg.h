#pragma once

/** Z80SubReg.h
 *
 * Instruction: SUB r
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80SubReg : public Z80Instruction
{
    public:
        Z80SubReg() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    // Calculate half-carry
                    r->operand.w = (r->af.h & 0x0F) 
                        + static_cast<uint8_t>(-*r->reg8[r->z] & 0x0F);
                    r->af.l = (r->operand.l & (FLAG_H | FLAG_3)) | FLAG_N; 
                                                                    // ...H3.1.
                    r->operand.w = r->af.h 
                        + static_cast<uint8_t>(-*r->reg8[r->z]);

                    r->af.l |= r->operand.l & (FLAG_S | FLAG_5);    // S.5H3.0.
                    r->af.l |= r->operand.h & FLAG_C;               // S.5H3.0C
                    r->af.l |=                                      // S.5H3P0C
                        (((r->af.h ^ r->operand.l)
                          & (-*r->reg8[r->z] ^ r->operand.l)) >> 5) & FLAG_PV;
                    r->af.l |= (r->operand.l) ? 0x00 : FLAG_Z;      // SZ5H3V0C
                    r->af.h = r->operand.l;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
