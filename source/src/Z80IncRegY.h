#pragma once

/** Z80IncRegY.h
 *
 * Instruction: INC ry
 *
 */

bool z80IncRegY()
{
    // Preserve carry flag.
    af.l &= FLAG_C;
    af.l |= incFlags[*regy8[y]];
    ++*regy8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
