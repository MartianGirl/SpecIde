#pragma once

/** Z80LdRegPtrWord.h
 *
 * Instruction: LD rr, (nn)
 *
 * Encoding: 11 101 101  01 rr1 011
 * M Cycles: 5 (ED, OCF, MRL, MRH, MRL, MRH)
 * T States: 20
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IY  10
 *  SP  11
 *
 */

bool z80LdRegPtrWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 4;
            memAddrMode = 0x00009811;
            return true;

        case 1:
            return true;

        case 2:
            tmp.w = iReg.w;
            return true;

        case 3:
            return true;

        case 4:
            *(regp[p]) = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
