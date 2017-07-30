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
            memWrCycles = 0;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            tmp.l = iReg.h;

            switch (y)
            {
                case 4: return ((af.l & FLAG_Z) == FLAG_Z);
                case 5: return ((af.l & FLAG_Z) == 0x00);
                case 6: return ((af.l & FLAG_C) == FLAG_C);
                case 7: return ((af.l & FLAG_C) == 0x00);
                default: assert(false); return true;
            }

        case 2:
            tmp.h = ((tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            tmp.w += pc.w;
            return false;

        case 4:
        case 5:
            return false;

        case 6:
            pc.w = tmp.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
