#pragma once

/** Z80AndRegY.h
 *
 * Instruction: AND ry
 *
 */

bool z80AndRegY()
{
    af.l = andFlags[af.h][*regy8[z]];
    af.h &= *regy8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
