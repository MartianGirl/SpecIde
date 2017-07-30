#pragma once

/** Z80PopReg.h
 *
 * Instruction: POP rr
 *
 * Encoding: 11 rr0 001
 * M Cycles: 3 (OCF, MRL, MRH)
 * T States: 10
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  HL  10
 *  AF  11
 *
 */

bool z80PopReg()
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
            *(regp2[p]) = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
