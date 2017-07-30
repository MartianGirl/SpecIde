#pragma once

/** Z80Im.h
 *
 * Instruction: IM 0
 * Instruction: IM 1
 * Instruction: IM 2
 *
 */

bool z80Im()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            if ((y & 0x02) == 0x00)
                im = 0;
            else if ((y & 0x01) == 0x00)
                im = 1;
            else
                im = 2;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
