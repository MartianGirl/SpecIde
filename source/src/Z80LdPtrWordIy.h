#pragma once

/** Z80LdPtrWordIy.h
 *
 * Instruction: LD (nn), IY
 *
 * Encoding: 11 111 101  00 100 010
 * M Cycles: 6 (FD, OCF, MRL, MRH, MWL, MWH)
 * T States: 20
 *
 */

bool z80LdPtrWordIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00009811;
            return true;

        case 1:
            return true;

        case 2:
            memWrCycles = 2;
            wz.w = iReg.w;
            oReg.w = iy.w;
            return true;

        case 3:
            return true;

        case 4:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
