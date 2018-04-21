#pragma once

/** Z80DecReg.h
 *
 * Instruction: DEC r
 *
 */

bool z80DecReg()
{
    af.l &= FLAG_C;
    af.l |= decFlags[*reg8[y]];
    --*reg8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
