#pragma once

/** Z80Scf.h
 *
 * Instruction: SCF
 *
 */

bool z80Scf()
{
    af.l &= (FLAG_S | FLAG_Z | FLAG_PV);
    af.l |= af.h & (FLAG_5 | FLAG_3);
    af.l |= FLAG_C;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
