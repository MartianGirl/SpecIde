#pragma once

/** Z80SetNPtrHl.h
 *
 * Instruction: SET n, (HL)
 *
 * Encoding: 11 001 011  11 bbb 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 */

bool z80SetNPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.l = iReg.h;
            acc.l |= (1 << y);
            return false;

        case 2:
            oReg.l = acc.l;
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
