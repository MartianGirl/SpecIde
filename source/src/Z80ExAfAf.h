#pragma once

/** Z80ExAfAf.h
 *
 * Instruction: EX AF, AF'
 *
 * Encoding: 00 001 000
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

#include "Z80Instruction.h"
#include "Z80RegisterSet.h"

class Z80ExAfAf : public Z80Instruction
{
    public:
        Z80ExAfAf() {}

        bool operator()(Z80RegisterSet* r)
        {
            switch (r->executionStep)
            {
                case 0:
                    r->memAddrMode = 0x00000000;

                    r->acc.w = r->af.w;
                    r->af.w = r->af_.w;
                    r->af_.w = r->acc.w;
                    r->prefix = PREFIX_NO;
                    return true;

                default:    // Should not happen
                    assert(false);
                    return true;
            }
        }
};

// vim: et:sw=4:ts=4
