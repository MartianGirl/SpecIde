#pragma once

/** Z80SrlReg.h
 *
 * Instruction: SRL r
 *
 * Encoding: 11 001 011  00 111 rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 * Flags: SZ503P0C
 * - 0 is shifted into MSB.
 *
 */

bool z80SrlReg()
{
    acc.w = *reg8[z];
    af.l = acc.l & FLAG_C;
    acc.w >>= 1;
    *reg8[z] = acc.h = acc.l;
    acc.h ^= acc.h >> 1;
    acc.h ^= acc.h >> 2;
    acc.h ^= acc.h >> 4;
    af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
    af.l |= (acc.l) ? 0x00 : FLAG_Z;
    af.l |= (acc.h & 0x01) ? 0x00 : FLAG_PV;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
