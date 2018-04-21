#pragma once

/** Z80IncRegX.h
 *
 * Instruction: INC rx
 *
 */

bool z80IncRegX()
{
    // Preserve carry flag.
    af.l &= FLAG_C;
    af.l |= incFlags[*regx8[y]];
    ++*regx8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
