#pragma once

/** Z80Di.h
 *
 * Instruction: DI
 *
 */

bool z80Di()
{
    iff &= ~(IFF1 | IFF2);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
