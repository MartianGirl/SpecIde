#pragma once

/** Z80IncPtrIy.h
 *
 * Instruction: INC (IY+d)
 *
 */

bool z80IncPtrIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000771;
            return true;

        case 1:
            wz.l = iReg.h;
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += iy.w;
            return false;

        case 4:
        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            // Preserve carry flag.
            af.l &= FLAG_C;
            af.l |= incFlags[iReg.h];
            return false;

        case 8:
            oReg.l = iReg.h + 1;
            memWrCycles = 1;
            return true;

        case 9:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
