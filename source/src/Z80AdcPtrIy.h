#pragma once

/** Z80AdcPtrIy.h
 *
 * Instruction: ADC A, (IY+d)
 *
 */

bool z80AdcPtrIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000071;
            return true;

        case 1:
            cpuProcCycles = 1;
            return true;

        case 2:
            wz.l = iReg.h;
            return false;

        case 3:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 4:
            wz.w += iy.w;
            return false;

        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
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
