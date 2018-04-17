#pragma once

/** Z80Nop.h
 *
 * Instruction: NOP
 *
 * Encoding: 00 000 000
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 */

bool z80Nop()
{
    prefix = PREFIX_NO;
    return true;
}

// vim: et:sw=4:ts=4
