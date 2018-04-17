#pragma once

/** Z80Ccf.h
 *
 * Instruction: CCF
 *
 */

bool z80Ccf()
{
    af.l &= (FLAG_S | FLAG_Z | FLAG_PV | FLAG_C);
    af.l |= af.h & (FLAG_5 | FLAG_3);
    af.l |= ((af.l & FLAG_C) << 4) & FLAG_H;
    af.l ^= FLAG_C;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
