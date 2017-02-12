#pragma once

/** Z80LdPtrBcA.h
 *
 * Instruction: LD (BC), A
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdPtrBcA : public Z80Instruction
{
    public:
        Z80LdPtrBcA() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 0;
                    r->memWrCycles = 1;
                    r->memAddrMode = 0x00000003;

                    r->oReg.l = r->af.h;
                    return true;

                case 1:
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
