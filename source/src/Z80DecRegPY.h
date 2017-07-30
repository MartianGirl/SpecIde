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
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;
            return false;

        case 1:
            // Decrement operand.
            --*regpy[p];
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
