#pragma once

/** Z80IncRegP.h
 *
 * Instruction: INC rr
 *
 */

bool z80IncRegP()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            ++*regp[p];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
