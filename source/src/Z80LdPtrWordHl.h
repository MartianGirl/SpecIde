#pragma once

/** Z80LdPtrWordHl.h
 *
 * Instruction: LD (nn), HL
 *
 * Encoding: 00 100 010
 * M Cycles: 5 (OCF, MRL, MRH, MWL, MWH)
 * T States: 16
 *
 */

bool z80LdPtrWordHl()
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
            tmp.w = iReg.w;
            oReg.w = hl.w;
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
