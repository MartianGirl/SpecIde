#pragma once

/** Z80ExDeHl.h
 *
 * Instruction: EX DE, HL
 *
 * Encoding: 11 101 011
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80ExDeHl()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = de.w;
            de.w = hl.w;
            hl.w = acc.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
