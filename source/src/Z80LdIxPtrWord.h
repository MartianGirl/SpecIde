#pragma once

/** Z80LdIxPtrWord.h
 *
 * Instruction: LD IX, (nn)
 *
 * Encoding: 11 011 101  00 101 010
 * M Cycles: 5 (DD, OCF, MRL, MRH, MRL, MRH)
 * T States: 20
 *
 */

bool z80LdIxPtrWord()
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
            wz.w = iReg.w;
            return true;

        case 3:
            return true;

        case 4:
            ix.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
