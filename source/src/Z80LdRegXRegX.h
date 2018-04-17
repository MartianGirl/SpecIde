#pragma once

/** Z80LdRegXRegX.h
 *
 * Instruction: LD r, s (DD-Prefixed)
 *
 * Encoding: 11 011 101  01 rrr sss
 * M Cycles: 2 (DD, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IXh 100
 * IXl 101
 *  A  111
 *
 */

bool z80LdRegXRegX()
{
    *(regx8[y]) = *(regx8[z]);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
