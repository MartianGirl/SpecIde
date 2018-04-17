#pragma once

/** Z80AndReg.h
 *
 * Instruction: AND r
 *
 */

bool z80AndReg()
{
    acc.l = acc.h = af.h & *reg8[z];
    acc.h ^= acc.h >> 1;
    acc.h ^= acc.h >> 2;
    acc.h ^= acc.h >> 4;
    af.l = (acc.h & 0x01) 
        ? FLAG_H : FLAG_H | FLAG_PV;                // ...H.P00
    af.l |=
        acc.l & (FLAG_S | FLAG_5 | FLAG_3);      // S.5H3P00
    af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3P00
    af.h = acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
