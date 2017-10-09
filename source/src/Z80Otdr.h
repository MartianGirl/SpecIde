#pragma once

/** Z80Otdr.h
 *
 * Instruction: OTDR
 *
 * Encoding: 11 101 101  10 111 011
 * M Cycles: 5 (OCF, OCF(5), MRB, IOW, CPU(5)) if B != 00h
 *           4 (OCF, OCF(5), MRB, IOW)         if B == 00h
 * T States: 21 if B != 00h
 *           16 if B == 00h
 *
 * Flags:
 * - Let's call k = ((HL) + L)
 * - C, H are set if k > FFh
 * - P is the parity of ((k & 7h) XOR B)
 * - N is bit 7 of (HL)
 * - S, Z, 5 and 3 are set by the result of decrementing B.
 * - S, Z, 5 and 3 at the end of the instruction are 0, 1, 0, 0.
 *
 * Notes:
 * - BC register is placed on the bus before B is decremented.
 *
 */

bool z80Otdr()
{
    switch (executionStep)
    {
        case 0:
            ioWrCycles = 1;
            memRdCycles = 1;
            memAddrMode = 0x000000E2;
            return false;

        case 1:
            return true;

        case 2:
            --hl.w;
            wz.w = bc.w;
            oReg.l = iReg.h;
            return true;

        case 3:
            --bc.h;
            af.l = bc.h & (FLAG_S | FLAG_5 | FLAG_3); // S.5.3...
            af.l |= (bc.h) ? 0x00 : FLAG_Z;           // SZ5.3...

            af.l |= (iReg.h & 0x80) >> 6;              // SZ5.3.N.
            acc.w = iReg.h + hl.l;
            af.l |= (acc.h) ? FLAG_H | FLAG_C : 0x00; // SZ5H3.NC
            acc.w = (acc.w & 0x07) ^ bc.h;
            acc.l ^= acc.l >> 1;
            acc.l ^= acc.l >> 2;
            acc.l ^= acc.l >> 4;
            af.l |= (acc.l & 0x01) ? 0x00 : FLAG_PV;   // SZ5H3PNC

            wz.w -= 0x0102;

            if (bc.h != 0x00)
                cpuProcCycles = 1;
            else
                prefix = PREFIX_NO;
            return true;

        case 4:
        case 5:
        case 6:
        case 7:
            return false;

        case 8:
            pc.w -= 0x0002;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
