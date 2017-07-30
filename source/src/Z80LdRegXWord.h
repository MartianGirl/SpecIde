#pragma once

/** Z80LdRegXWord.h
 *
 * DD Prefixed instructions.
 * Instruction: LD BC, nn
 * Instruction: LD DE, nn
 * Instruction: LD IX, nn
 * Instruction: LD SP, nn
 *
 * Encoding: 11 011 101  00 rr0 001
 * M Cycles: 4 (DD, OCF, MRL, MRH)
 * T States: 14
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IX  10
 *  SP  11
 *
 */

bool z80LdRegXWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000011;
            return true;

        case 1:
            return true;

        case 2:
            *regpx[p] = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
