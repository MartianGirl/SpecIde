#pragma once

/** Z80SubReg.h
 *
 * Instruction: SUB r
 *
 */

bool z80SubReg()
{
    af.l = subFlags[0][af.h][*reg8[z]];
    af.h -= *reg8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
