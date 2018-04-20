#pragma once

/** Z80AndRegX.h
 *
 * Instruction: AND rx
 *
 */

bool z80AndRegX()
{
    af.l = andFlags[af.h][*regx8[z]];
    af.h &= *regx8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
