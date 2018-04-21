#pragma once

/** Z80IncReg.h
 *
 * Instruction: INC r
 *
 */

bool z80IncReg()
{
    af.l &= FLAG_C;
    af.l |= incFlags[*reg8[y]];
    ++*reg8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
