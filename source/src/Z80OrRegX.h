#pragma once

/** Z80OrRegX.h
 *
 * Instruction: OR rx
 *
 */

bool z80OrRegX()
{
    af.l = orFlags[af.h][*regx8[z]];
    af.h |= *regx8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
