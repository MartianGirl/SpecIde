#pragma once

/** Z80LdRegXRegX.h
 *
 * Instruction: LD r, r' (DD-Prefixed)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegXRegX : public Z80Instruction
{
    public:
        Z80LdRegXRegX() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    *(r->regx8[r->y]) = *(r->regx8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
