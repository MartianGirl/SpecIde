#pragma once

/** Z80LdRegYByte.h
 *
 * Instruction: FD-prefixed LD r, n
 *
 * Encoding: 11 111 101  00 rrr 110
 * M Cycles: 3 (FD, OCF, MRB)
 * T States: 11
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IYh 100
 * IYl 101
 *  A  111
 *
 */

bool z80LdRegYByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            *(regy8[y]) = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
