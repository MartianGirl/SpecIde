#pragma once

/** Z80LdSpIx.h
 *
 * Instruction: LD SP, IX
 *
 * Encoding: 11 011 101  11 111 001
 * M Cycles: 2 (DD, OCF)
 * T States: 10
 *
 */

bool z80LdSpIx()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            sp.w = ix.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;

    }
}

// vim: et:sw=4:ts=4
