#pragma once

/** Z80LdRegPtrIy.h
 *
 * Instruction: LD r, (IY+d)
 *
 * Encoding: 11 111 101  01 rrr 110
 * M Cycles: 5 (FD, OCF, MRB, CPU, MRB)
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

bool z80LdRegPtrIy()
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
            tmp.l = iReg.h;
            return false;

        case 3:
            tmp.h = ((tmp.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 4:
            tmp.w += iy.w;
            return false;

        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            *(reg8[y]) = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
