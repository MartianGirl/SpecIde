#pragma once

/** Z80LdSpHl.h
 *
 * Instruction: LD SP, HL
 *
 * Encoding: 11 111 001
 * M Cycles: 1 (OCF)
 * T States: 6
 *
 */

bool z80LdSpHl()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            sp.w = hl.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;

    }
}

// vim: et:sw=4:ts=4
