#pragma once

/** Z80LdRegPtrIy.h
 *
 * Instruction: LD r, (IY + d)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegPtrIy : public Z80Instruction
{
    public:
        Z80LdRegPtrIy() {}

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
                    r->operand.w >>= 8;
                    return false;

                case 2:
                    r->operand.h = ((r->operand.l & 0x80) == 0x80) ? 0xFF : 0x00;
                    return false;

                case 3:
                    r->operand.w += r->iy.w;
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
