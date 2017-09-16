#pragma once

/** Z80OutPtrCReg.h
 *
 * Instruction: OUT (C), r
 *
 * Encoding: 11 101 101  01 rrr 001
 * M Cycles: 3 (ED, OCF, IOWR)
 * T States: 12
 *
 *  Reg  r
 *  --- ---
 *   B  000
 *   C  001
 *   D  010
 *   E  011
 *   H  100
 *   L  101
 *   -  110     * Outputs 00h
 *   A  111
 *
 */

bool z80OutPtrCReg()
{
    switch (executionStep)
    {
        case 0:
            ioWrCycles = 1;
            memAddrMode = 0x0000000E;

            tmp.w = bc.w;
            oReg.l = (y == 6) ? 0x00 : *reg8[y];
            return true;
        case 1:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
