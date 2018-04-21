#pragma once

/** Z80IncRegPY.h
 *
 * Instruction: INC ry
 *
 */

bool z80IncRegPY()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            ++*regpy[p];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
