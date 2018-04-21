#pragma once

/** Z80DecPtrIx.h
 *
 * Instruction: DEC (IX+d)
 *
 */

bool z80DecPtrIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000661;
            return true;

        case 1:
            wz.l = iReg.h;
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += ix.w;
            return false;

        case 4:
        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            // Preserve carry bit.
            af.l &= FLAG_C;
            af.l |= decFlags[iReg.h];
            return false;

        case 8:
            oReg.l = iReg.h - 1;
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
