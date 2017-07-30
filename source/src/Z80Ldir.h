#pragma once

/** Z80Ldir.h
 *
 * Instruction: LDIR
 *
 * Encoding: 11 101 101  10 110 000
 * M Cycles: 5 (OCF, OCF, MRB, MWB(5), CPU(5)) if BC != 0000h
 *           4 (OCF, OCF, MRB, MWB(5))         if BC == 0000h
 * T States: 21 if BC != 0000h
 *           16 if BC == 0000h
 *
 * Flags:
 * - Let's call 'n' the value of the byte being transferred plus the value
 *   of register A.
 * - S, Z, C are not changed.
 * - H is always zero.
 * - P/V is set if BC != 0000h
 * - 3 is bit 3 of n.
 * - 5 is bit 1 of n.
 *
 */

bool z80Ldir()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x00000042;
            return true;

        case 1:
            oReg.l = iReg.h;
            return true;

        case 2:
            --bc.w;
            ++de.w;
            ++hl.w;
            return false;

        case 3:
            tmp.l = iReg.h + af.h;
            af.l &= FLAG_S | FLAG_Z | FLAG_C;            // SZ00000C
            af.l |= (tmp.l & FLAG_3);                 // SZ00300C
            af.l |= (tmp.l << 4) & FLAG_5;            // SZ50300C
            af.l |= (bc.w) ? FLAG_PV : 0x00;          // SZ503P0C
            return false;

        case 4:
            if (bc.w != 0x0000)
                cpuProcCycles = 1;
            else
                prefix = PREFIX_NO;
            return true;

        case 5:
        case 6:
        case 7:
        case 8:
            return false;

        case 9:
            pc.w -= 0x0002;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
