#pragma once

/** Z80AddRegX.h
 *
 * Instruction: ADD A, rx
 *
 */

bool z80AddRegX()
{
    af.l = addFlags[0][af.h][*regx8[z]];
    af.h += *regx8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
