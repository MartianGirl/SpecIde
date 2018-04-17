#pragma once

/** Z80JpIx.h
 *
 * JP (IX) Instruction.
 *
 */

bool z80JpIx()
{
    pc.w = ix.w;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
