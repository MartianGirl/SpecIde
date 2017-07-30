#pragma once

/** Z80LdRegXByte.h
 *
 * Instruction: DD-prefixed LD r, n
 *
 * Encoding: 11 011 101  00 rrr 110
 * M Cycles: 3 (DD, OCF, MRB)
 * T States: 11
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 * IXh 100
 * IXl 101
 *  A  111
 *
 */

bool z80LdRegXByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            *(regx8[y]) = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
