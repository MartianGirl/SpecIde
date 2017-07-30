#pragma once

/** Z80Exx.h
 *
 * Instruction: EXX
 *
 * Encoding: 11 011 001
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80Exx()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = bc.w;
            bc.w = bc_.w;
            bc_.w = acc.w;

            acc.w = de.w;
            de.w = de_.w;
            de_.w = acc.w;

            acc.w = hl.w;
            hl.w = hl_.w;
            hl_.w = acc.w;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
