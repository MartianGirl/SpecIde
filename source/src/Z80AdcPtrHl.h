#pragma once

/** Z80AdcPtrHl.h
 *
 * Instruction: ADC A, (HL)
 *
 */

bool z80AdcPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
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
