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
    tmp.w = bc.w;
    bc.w = bc_.w;
    bc_.w = tmp.w;

    tmp.w = de.w;
    de.w = de_.w;
    de_.w = tmp.w;

    tmp.w = hl.w;
    hl.w = hl_.w;
    hl_.w = tmp.w;

    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
