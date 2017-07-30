#pragma once

/** Z80Nop.h
 *
 * Instruction: NOP
 *
 * Encoding: 00 000 000
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80Nop()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
