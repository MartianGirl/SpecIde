#pragma once

/** Z80Ei.h
 *
 * Instruction: EI
 *
 */

bool z80Ei()
{
    iff |= (IFF1 | IFF2);
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
