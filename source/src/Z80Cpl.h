#pragma once

/** Z80Cpl.h
 *
 * Instruction: CPL
 *
 */

bool z80Cpl()
{
    af.h = ~af.h;
    af.l &= (FLAG_S | FLAG_Z | FLAG_PV | FLAG_C);
    af.l |= af.h & (FLAG_5 | FLAG_3);
    af.l |= FLAG_H | FLAG_N;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
