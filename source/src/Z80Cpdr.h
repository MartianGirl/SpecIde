#pragma once

/** Z80Cpdr.h
 *
 * Instruction: CPDR
 *
 * Encoding: 11 101 101  10 111 001
 * M Cycles: 4 (OCF, OCF, MRB, CPU(5), CPU(5)) if BC != 0000h
 * M Cycles: 4 (OCF, OCF, MRB, CPU(5))         if BC == 0000h
 * T States: 21 if BC != 0000h
 * T States: 16 if BC == 0000h
 *
 * Flags:
 * - Let's subtract from A the value in (HL).
 * - S, Z, H are set by this subtraction.
 * - Now, let's call 'n' the value A - (HL) - H.
 * - 3 is bit 3 of n.
 * - 5 is bit 1 of n.
 * - C is not changed.
 * - N is always set.
 * - P/V is set if BC != 0000h
 *
 */

bool z80Cpdr()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            return true;

        case 2:
            --bc.w;
            --hl.w;
            return false;

        case 3:
            acc.l = af.h - iReg.h;
            af.l &= FLAG_C;                              // 0000000C
            af.l |= FLAG_N;                              // 000000NC
            af.l |= 
                (af.h ^ iReg.h ^ acc.l) & FLAG_H;   // 000H00NC
            af.l |= acc.l & FLAG_S;                   // S00H00NC
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ0H00NC
            return false;

        case 4:
            acc.l -= (af.l & FLAG_H) >> 4;
            af.l |= (acc.l & FLAG_3);                 // SZ0H30NC
            af.l |= (acc.l << 4) & FLAG_5;            // SZ5H30NC
            return false;

        case 5:
            af.l |= (bc.w) ? FLAG_PV : 0x00;          // SZ5H3PNC
            --wz.w;
            return false;

        case 6:
            if (bc.w != 0x0000 && ((af.l & FLAG_Z) == 0x00))
                cpuProcCycles = 1;
            else
                prefix = PREFIX_NO;
            return true;

        case 7:
        case 8:
        case 9:
        case 10:
            return false;

        case 11:
            wz.w = pc.w - 1;
            pc.w = wz.w - 1;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
