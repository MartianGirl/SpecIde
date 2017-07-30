#pragma once

/** Z80PrefixCB.h
 *
 * Prefix CB.
 *
 */

bool z80PrefixCB()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            prefix = PREFIX_CB;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
