#pragma once

/** Z80CpReg.h
 *
 * Instruction: CP r
 *
 */

bool z80CpReg()
{
    acc.w = af.h - *reg8[z];

    // Flags 5 & 3 are copied from the operand.
    af.l = *reg8[z] & (FLAG_5 | FLAG_3);
    af.l |= acc.l & FLAG_S;
    af.l |= acc.h & FLAG_C;
    af.l |= FLAG_N;
    af.l |= (acc.l ^ af.h ^ *reg8[z]) & FLAG_H;
    af.l |= (((acc.l ^ *reg8[z] ^ af.h) >> 5) 
            ^ (acc.h << 2)) & FLAG_PV;
    af.l |= acc.l ? 0x00 : FLAG_Z;            // SZ5H3VNC
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
