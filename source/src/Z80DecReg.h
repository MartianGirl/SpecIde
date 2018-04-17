#pragma once

/** Z80DecReg.h
 *
 * Instruction: DEC r
 *
 */

bool z80DecReg()
{
    acc.w = *reg8[y] - 1;

    // Preserve carry flag.
    af.l &= FLAG_C;
    af.l |= FLAG_N;
    af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
    af.l |= (acc.l ^ *reg8[y]) & FLAG_H;
    af.l |= (((acc.l ^ *reg8[y]) >> 5)
            ^ (acc.h << 2)) & FLAG_PV;
    af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
    *reg8[y] = acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
