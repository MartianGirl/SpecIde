#pragma once

/** Z80AndReg.h
 *
 * Instruction: AND r
 *
 */

bool z80AndReg()
{
    af.l = andFlags[af.h][*reg8[z]];
    af.h &= *reg8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
