#pragma once

/** Z80IncRegPX.h
 *
 * Instruction: INC rx
 *
 */

bool z80IncRegPX()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            ++*regpx[p];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
