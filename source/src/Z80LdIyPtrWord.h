#pragma once

/** Z80LdIyPtrWord.h
 *
 * Instruction: LD IY, (nn)
 *
 * Encoding: 11 111 101  00 101 010
 * M Cycles: 5 (FD, OCF, MRL, MRH, MRL, MRH)
 * T States: 20
 *
 */

bool z80LdIyPtrWord()
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
            iy.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
