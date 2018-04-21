#pragma once

/** Z80SubRegX.h
 *
 * Instruction: SUB rx
 *
 */

bool z80SubRegX()
{
    af.l = subFlags[0][af.h][*regx8[z]];
    af.h -= *regx8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
