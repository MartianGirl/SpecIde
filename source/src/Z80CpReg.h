#pragma once

/** Z80CpReg.h
 *
 * Instruction: CP r
 *
 */

bool z80CpReg()
{
    af.l = cpFlags[af.h][*reg8[z]];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
