#pragma once

/** Z80PrefixDD.h
 *
 * Prefix DD.
 *
 */

bool z80PrefixDD()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            prefix = PREFIX_DD;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
