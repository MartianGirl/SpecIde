#pragma once

/** Z80JrByte.h
 *
 * JR n Instruction.
 *
 */

bool z80JrByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            wz.l = iReg.h;
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += pc.w;
            return false;

        case 4:
            return false;

        case 5:
            return false;

        case 6:
            pc.w = wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
