#pragma once

/** Z80JpHl.h
 *
 * JP (HL) Instruction.
 *
 */

bool z80JpHl()
{
    pc.w = hl.w;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
