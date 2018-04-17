#pragma once

/** Z80JpIy.h
 *
 * JP (IY) Instruction.
 *
 */

bool z80JpIy()
{
    pc.w = iy.w;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
