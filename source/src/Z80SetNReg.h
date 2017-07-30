#pragma once

/** Z80SetNReg.h
 *
 * Instruction: SET n, r
 *
 * Encoding: 11 001 011  11 bbb rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
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

bool z80SetNReg()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            *reg8[z] |= (1 << y);

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
