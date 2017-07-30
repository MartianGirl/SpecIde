#pragma once

/** Z80LdRegReg.h
 *
 * Instruction: LD r, s
 *
 * Encoding: 01 rrr sss
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80LdRegReg()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            *(reg8[y]) = *(reg8[z]);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
