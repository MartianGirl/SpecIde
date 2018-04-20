#pragma once

/** Z80AddReg.h
 *
 * Instruction: ADD A, r
 *
 */

bool z80AddReg()
{
    af.l = addFlags[0][af.h][*reg8[z]];
    af.h += *reg8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
