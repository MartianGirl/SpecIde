#pragma once

/** Z80DecRegP.h
 *
 * Instruction: DEC rr
 *
 */

bool z80DecRegP()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            return false;

        case 1:
            // Decrement operand.
            --*regp[p];
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
