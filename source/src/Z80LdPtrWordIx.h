#pragma once

/** Z80LdPtrWordIx.h
 *
 * Instruction: LD (nn), IX
 *
 * Encoding: 11 011 101  00 100 010
 * M Cycles: 6 (DD, OCF, MRL, MRH, MWL, MWH)
 * T States: 20
 *
 */

bool z80LdPtrWordIx()
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
            oReg.w = ix.w;
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
