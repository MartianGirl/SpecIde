#pragma once

/** Z80DecRegX.h
 *
 * Instruction: DEC rx
 *
 */

bool z80DecRegX()
{
    af.l &= FLAG_C;
    af.l |= decFlags[*regx8[y]];
    --*regx8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
