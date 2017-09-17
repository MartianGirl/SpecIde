#pragma once

/** Z80JrCcByte.h
 *
 * JR NZ, n Instruction.
 * JR Z, n Instruction.
 * JR NC, n Instruction.
 * JR C, n Instruction.
 *
 */

bool z80JrCcByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            wz.l = iReg.h;

            switch (y)
            {
                case 4: cpuProcCycles = ((af.l & FLAG_Z) == FLAG_Z) ? 0 : 1; break;
                case 5: cpuProcCycles = ((af.l & FLAG_Z) == 0x00) ? 0 : 1; break;
                case 6: cpuProcCycles = ((af.l & FLAG_C) == FLAG_C) ? 0 : 1; break;
                case 7: cpuProcCycles = ((af.l & FLAG_C) == 0x00) ? 0 : 1; break;
                default: assert(false);
            }
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += pc.w;
            return false;

        case 4:
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
