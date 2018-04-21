#pragma once

/** Z80CpRegY.h
 *
 * Instruction: CP ry
 *
 */

bool z80CpRegY()
{
    af.l = cpFlags[af.h][*regy8[z]];
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
