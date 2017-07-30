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
            memWrCycles = 0;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            tmp.l = iReg.h;
            return false;

        case 2:
            tmp.h = ((tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            tmp.w += pc.w;
            return false;

        case 4:
            return false;

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
