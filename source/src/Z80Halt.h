#pragma once

/** Z80Halt.h
 *
 * Instruction: HALT
 *
 */

bool z80Halt()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;
            iff |= HALT;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
