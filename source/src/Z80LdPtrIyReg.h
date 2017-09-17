#pragma once

/** Z80LdPtrIyReg.h
 *
 * Instruction: LD (IY+d), r
 *
 * Encoding: 11 111 101  01 110 rrr
 * M Cycles: 5 (FD, OCF, MRB, CPU, MWB)
 * T States: 19
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80LdPtrIyReg()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000071;
            return true;

        case 1: // Memory read byte cycle
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
            memWrCycles = 1;
            oReg.l = *(reg8[z]);
            return true;

        case 7:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
