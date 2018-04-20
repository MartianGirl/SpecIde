#pragma once

/** Z80AdcRegX.h
 *
 * Instruction: ADC A, rx
 *
 */

bool z80AdcRegX()
{
    acc.l = af.l & FLAG_C;
    af.l = addFlags[acc.l][af.h][*regx8[z]];
    af.h += *regx8[z] + acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
