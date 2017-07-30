#pragma once

/** Z80LdIA.h
 *
 * Instruction: LD I, A
 *
 * Encoding: 11 101 101  01 000 111
 * M Cycles: 2 (ED, OCF)
 * T States: 9
 *
 */

bool z80LdIA()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            return false;

        case 1:
            ir.h = af.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;

    }
}

// vim: et:sw=4:ts=4
