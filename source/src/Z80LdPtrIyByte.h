#pragma once

/** Z80LdPtrIyByte.h
 *
 * Instruction: LD (IY+d), n
 *
 * Encoding: 11 111 101  00 110 110
 * M Cycles: 5 (FD, OCF, MRB, MRB/CPU, MWB)
 * T States: 19
 *
 */

bool z80LdPtrIyByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000711;
            return true;

        case 1:
            tmp.l = iReg.h;
            return true;

        case 2:
            tmp.h = ((tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            tmp.w += iy.w;
            return false;

        case 4:
            memWrCycles = 1;
            oReg.l = iReg.h;
            return true;

        case 5:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
