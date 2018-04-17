#pragma once

/** Z80Rrca.h
 *
 * Instruction: RRCA
 *
 * Encoding: 00 001 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - LSB is copied into MSB and into CF.
 *
 */

bool z80Rrca()
{
    acc.h = af.h & 0x01;
    acc.l = af.h;
    af.l &= FLAG_S | FLAG_Z | FLAG_PV;
    af.l |= acc.h & FLAG_C;
    acc.w >>= 1;
    af.h = acc.l;
    af.l |= af.h & (FLAG_5 | FLAG_3);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
