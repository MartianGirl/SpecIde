#pragma once

/** Z80Rst.h
 *
 * Instruction: RST 0h
 * Instruction: RST 8h
 * Instruction: RST 10h
 * Instruction: RST 18h
 * Instruction: RST 20h
 * Instruction: RST 28h
 * Instruction: RST 30h
 * Instruction: RST 38h
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80Rst : public Z80Instruction
{
    public:
        Z80Rst() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 2;
                    r->memAddrMode = 0x000000AA;

                    r->oReg.l = r->pc.h;
                    r->oReg.h = r->pc.l;
                    return true;

                case 1:
                    return true;

                case 2:
                    return false;

                case 3:
                    r->pc.w = (r->y << 3);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
