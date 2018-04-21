#pragma once

/** Z80XorRegX.h
 *
 * Instruction: XOR rx
 *
 */

bool z80XorRegX()
{
    af.l = xorFlags[af.h][*regx8[z]];
    af.h ^= *regx8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
