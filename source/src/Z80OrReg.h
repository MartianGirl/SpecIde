#pragma once

/** Z80OrReg.h
 *
 * Instruction: OR r
 *
 */

bool z80OrReg()
{
    af.l = orFlags[af.h][*reg8[z]];
    af.h |= *reg8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
