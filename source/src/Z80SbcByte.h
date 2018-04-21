#pragma once

/** Z80SbcByte.h
 *
 * Instruction: SBC A, n
 *
 */

bool z80SbcByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            acc.l = af.l & FLAG_C;
            af.l = subFlags[acc.l][af.h][iReg.h];
            af.h -= iReg.h + acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
