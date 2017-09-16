#pragma once

/** Z80LdHlPtrWord.h
 *
 * Instruction: LD HL, (nn)
 *
 * Encoding: 00 101 010
 * M Cycles: 5 (OCF, MRL, MRH, MRL, MRH)
 * T States: 16
 *
 */

bool z80LdHlPtrWord()
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
            hl.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
