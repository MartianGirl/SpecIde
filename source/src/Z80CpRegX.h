#pragma once

/** Z80CpRegX.h
 *
 * Instruction: CP rx
 *
 */

bool z80CpRegX()
{
    af.l = cpFlags[af.h][*regx8[z]];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
