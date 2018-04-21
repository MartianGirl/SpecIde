#pragma once

/** Z80DecRegPX.h
 *
 * Instruction: DEC rx
 *
 */

bool z80DecRegPX()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            --*regpx[p];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
