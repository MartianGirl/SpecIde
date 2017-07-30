#pragma once

/** Z80LdPtrHlReg.h
 *
 * Instruction: LD (HL), r
 *
 * Encoding: 01 110 rrr
 * M Cycles: 2 (OCF, MWB)
 * T States: 7
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80LdPtrHlReg()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 1;
            memAddrMode = 0x00000002;

            oReg.l = *(reg8[z]);
            return true;

        case 1:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
