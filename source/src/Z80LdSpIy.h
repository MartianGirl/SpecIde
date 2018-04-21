#pragma once

/** Z80LdSpIy.h
 *
 * Instruction: LD SP, IY
 *
 * Encoding: 11 111 101  11 111 001
 * M Cycles: 2 (FD, OCF)
 * T States: 10
 *
 */

bool z80LdSpIy()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            sp.w = iy.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;

    }
}

// vim: et:sw=4:ts=4
