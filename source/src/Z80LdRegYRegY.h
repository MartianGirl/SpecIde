#pragma once

/** Z80LdRegYRegY.h
 *
 * Instruction: LD r, s (FD-Prefixed)
 *
 * Encoding: 11 111 101  01 rrr sss
 * M Cycles: 2 (FD, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IYh 100
 * IYl 101
 *  A  111
 *
 */

bool z80LdRegYRegY()
{
    *(regy8[y]) = *(regy8[z]);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
