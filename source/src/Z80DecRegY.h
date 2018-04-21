#pragma once

/** Z80DecRegY.h
 *
 * Instruction: DEC ry
 *
 */

bool z80DecRegY()
{
    af.l &= FLAG_C;
    af.l |= decFlags[*regy8[y]];
    --*regy8[y];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
