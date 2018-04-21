#pragma once

/** Z80XorRegY.h
 *
 * Instruction: XOR ry
 *
 */

bool z80XorRegY()
{
    af.l = xorFlags[af.h][*regy8[z]];
    af.h ^= *regy8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
