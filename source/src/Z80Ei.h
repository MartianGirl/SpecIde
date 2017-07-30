#pragma once

/** Z80Ei.h
 *
 * Instruction: EI
 *
 */

bool z80Ei()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            iff |= (IFF1 | IFF2);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
