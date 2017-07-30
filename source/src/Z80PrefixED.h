#pragma once

/** Z80PrefixED.h
 *
 * Prefix ED.
 *
 */

bool z80PrefixED()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            prefix = PREFIX_ED;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
