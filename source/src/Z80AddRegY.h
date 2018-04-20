#pragma once

/** Z80AddRegY.h
 *
 * Instruction: ADD A, ry
 *
 */

bool z80AddRegY()
{
    af.l = addFlags[0][af.h][*regy8[z]];
    af.h += *regy8[z];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
