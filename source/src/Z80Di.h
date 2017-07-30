#pragma once

/** Z80Di.h
 *
 * Instruction: DI
 *
 */

bool z80Di()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            iff &= ~(IFF1 | IFF2);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
