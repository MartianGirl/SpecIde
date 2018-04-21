#pragma once

/** Z80OrRegY.h
 *
 * Instruction: OR ry
 *
 */

bool z80OrRegY()
{
    af.l = orFlags[af.h][*regy8[z]];
    af.h |= *regy8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
