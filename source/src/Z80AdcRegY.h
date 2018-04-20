#pragma once

/** Z80AdcRegY.h
 *
 * Instruction: ADC A, ry
 *
 */

bool z80AdcRegY()
{
    acc.l = af.l & FLAG_C;
    af.l = addFlags[acc.l][af.h][*regy8[z]];
    af.h += *regy8[z] + acc.l;
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
