#pragma once

/** Z80PopIx.h
 *
 * Instruction: POP IX
 *
 * Encoding: 11 011 101  11 100 001
 * M Cycles: 4 (DD, OCF, MRL, MRH)
 * T States: 15
 *
 */

bool z80PopIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x000000BB;
            return true;

        case 1:
            return true;

        case 2:
            ix.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
