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
            memAddrMode = 0x00000000;
            return false;

        case 1:
            // Increment operand.
            ++*regp[p];
            return false;

        case 2:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
