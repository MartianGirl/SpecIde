#pragma once

/** Z80PrefixFD.h
 *
 * Prefix FD.
 *
 */

bool z80PrefixFD()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            prefix = PREFIX_FD;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
