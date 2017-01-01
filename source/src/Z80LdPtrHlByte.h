#pragma once

/** Z80LdPtrHlByte.h
 *
 * Instruction: LD (HL), n
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrHlByte : public Z80Instruction
{
    public:
        Z80LdPtrHlByte() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000021;
                    return true;

                case 1:
                    return true;

                case 2:
                    return true;
                    r->prefix = PREFIX_NO;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
