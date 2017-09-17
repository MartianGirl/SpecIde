#pragma once

/** Z80LdAPtrWord.h
 *
 * Instruction: LD A, (nn)
 *
 * Encoding: 00 111 010
 * M Cycles: 4 (OCF, MRL, MRH, MRB)
 * T States: 13
 *
 */

bool z80LdAPtrWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 3;
            memAddrMode = 0x00000811;
            return true;

        case 1:
            return true;

        case 2:
            wz.w = iReg.w;
            return true;

        case 3:
            af.h = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
