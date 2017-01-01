#pragma once

/** Z80LdRegYRegY.h
 *
 * Instruction: LD r, r' (FD-Prefixed)
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdRegYRegY : public Z80Instruction
{
    public:
        Z80LdRegYRegY() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000000;

                    *(r->regy8[r->y]) = *(r->regy8[r->z]);
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
