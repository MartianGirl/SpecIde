#pragma once

/** Z80InAPtrByte.h
 *
 * Instruction: IN A, (n)
 *
 * Encoding: 11 011 011
 * M Cycles: 3 (OCF, MRB, IORD)
 * T States: 11
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
                    r->ioRdCycles = 1;
                    r->memAddrMode = 0x000000D1;
                    return true;

                case 1:
                    return true;

                case 2:
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
