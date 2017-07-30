#pragma once

/** Z80LdPtrHlByte.h
 *
 * Instruction: LD (HL), n
 *
 * Encoding: 00 110 110
 * M Cycles: 3 (OCF, MRB, MWB)
 * T States: 10
 *
 */

bool z80LdPtrHlByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000021;
            return true;

        case 1:
            oReg.l = iReg.h;
            return true;

        case 2:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
