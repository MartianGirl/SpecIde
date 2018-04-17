#pragma once

/** Z80Halt.h
 *
 * Instruction: HALT
 *
 */

bool z80Halt()
{
    iff |= HALT;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
