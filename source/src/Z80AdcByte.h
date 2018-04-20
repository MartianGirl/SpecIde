#pragma once

/** Z80AdcByte.h
 *
 * Instruction: ADC A, n
 *
 */

bool z80AdcByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            acc.l = af.l & FLAG_C;
            af.l = addFlags[acc.l][af.h][iReg.h];
            af.h += iReg.h + acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
