#pragma once

/** Z80LdPtrIxReg.h
 *
 * Instruction: LD (IX+d), r
 *
 * Encoding: 11 011 101  01 110 rrr
 * M Cycles: 5 (DD, OCF, MRB, CPU, MWB)
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

bool z80LdPtrIxReg()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000061;
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
            tmp.w += ix.w;
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
