#pragma once

/** Z80DecRegPY.h
 *
 * Instruction: DEC ry
 *
 */

bool z80DecRegPY()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            --*regpy[p];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
