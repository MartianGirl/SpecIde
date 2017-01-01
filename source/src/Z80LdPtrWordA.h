#pragma once

/** Z80LdPtrWordA.h
 *
 * Instruction: LD (nn), A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrWordA : public Z80Instruction
{
    public:
        Z80LdPtrWordA() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 2;
                    r->memWrCycles = 0;
                    r->memAddrMode = 0x00000811;
                    return true;

                case 1:
                    return true;

                case 2:
                    r->memWrCycles = 1;
                    r->outWord.l = r->af->h;
                    return true;

                case 3:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
