#pragma once

/** Z80LdPtrWordReg.h
 *
 * Instruction: LD (nn), rr
 *
 * Encoding: 11 101 101  01 rr0 011
 * M Cycles: 6 (ED, OCF, MRL, MRH, MWL, MWH)
 * T States: 20
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IY  10
 *  SP  11
 *
 */

bool z80LdPtrWordReg()
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
            oReg.w = *(regp[p]);
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
