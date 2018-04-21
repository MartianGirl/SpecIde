#pragma once

/** Z80SubRegY.h
 *
 * Instruction: SUB ry
 *
 */

bool z80SubRegY()
{
    af.l = subFlags[0][af.h][*regy8[z]];
    af.h -= *regy8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
