#pragma once

/** Z80LdPtrWordA.h
 *
 * Instruction: LD (nn), A
 *
 * Encoding: 00 110 010
 * M Cycles: 2 (OCF, MRL, MRH, MWB)
 * T States: 13
 *
 */

bool z80LdPtrWordA()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000811;
            return true;

        case 1:
            return true;

        case 2:
            tmp.w = iReg.w;
            memWrCycles = 1;
            oReg.l = af.h;
            return true;

        case 3:
            tmp.h = af.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
