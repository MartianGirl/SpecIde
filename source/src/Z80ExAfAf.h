#pragma once

/** Z80ExAfAf.h
 *
 * Instruction: EX AF, AF'
 *
 * Encoding: 00 001 000
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80ExAfAf()
{
    acc.w = af.w;
    af.w = af_.w;
    af_.w = acc.w;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
