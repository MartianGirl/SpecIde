#pragma once

/** Z80XorReg.h
 *
 * Instruction: XOR r
 *
 */

bool z80XorReg()
{
    af.l = xorFlags[af.h][*reg8[z]];
    af.h ^= *reg8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
