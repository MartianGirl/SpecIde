#pragma once

/** Z80AdcReg.h
 *
 * Instruction: ADC A, r
 *
 */

bool z80AdcReg()
{
    acc.l = af.l & FLAG_C;
    af.l = addFlags[acc.l][af.h][*reg8[z]];
    af.h += *reg8[z] + acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
