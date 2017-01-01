#pragma once

/** Z80LdRegPtrIx.h
 *
 * Instruction: LD r, (IX + d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegPtrIx : public Z80Instruction
{
    public:
        Z80LdRegPtrIx() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000061;
                    return true;

                case 1:
                    r->offset.w = r->operand.w >> 8;
                    return false;

                case 2:
                    r->offset.h = ((r->offset.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->offset.w += r->ix.w;
                    return false;

                case 4:
                    return false;

                case 5:
                    r->memRdCycles = 1;
                    return true;

                case 6:
                    *(r->reg8[r->y]) = r->operand.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
