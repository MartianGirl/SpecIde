#pragma once

/** Z80PopIy.h
 *
 * Instruction: POP IY
 *
 * Encoding: 11 111 101  11 100 001
 * M Cycles: 4 (FD, OCF, MRL, MRH)
 * T States: 15
 *
 */

bool z80PopIy()
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
            iy.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
