#pragma once

/** Z80LdAPtrBc.h
 *
 * Instruction: LD A, (BC)
 *
 * Encoding: 00 001 010
 * M Cycles: 2 (OCF, MRB)
 * T States: 7
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80LdAPtrBc : public Z80Instruction
{
    public:
        Z80LdAPtrBc() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memRdCycles = 1;
                    r->memAddrMode = 0x00000003;
                    return true;

                case 1:
                    r->af.h = r->iReg.h;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
