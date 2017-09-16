#pragma once

/** Z80Outd.h
 *
 * Instruction: OUTD
 *
 * Encoding: 11 101 101  10 101 011
 * M Cycles: 4 (OCF, OCF(5), MRB, IOW)
 * T States: 16
 *
 * Flags:
 * - Let's call k = ((HL) + L)
 * - C, H are set if k > FFh
 * - P is the parity of ((k & 7h) XOR B)
 * - N is bit 7 of (HL)
 * - S, Z, 5 and 3 are set by the result of decrementing B.
 *
 * Notes:
 * - BC register is placed on the bus before B is decremented.
 *
 */

bool z80Outd()
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
            tmp.w = bc.w;
            oReg.l = iReg.h;
            return true;

        case 3:
            --bc.h;
            af.l = bc.h & (FLAG_S | FLAG_5 | FLAG_3);// S.5H3PNC
            af.l |= (bc.h) ? 0x00 : FLAG_Z;           // SZ5H3PNC

            af.l |= (iReg.h & 0x80) >> 6;             // ......N.
            acc.w = iReg.h + hl.l;
            af.l |= (acc.h) ? FLAG_H | FLAG_C : 0x00; // ...H..NC
            acc.w = (acc.w & 0x07) ^ bc.h;
            acc.l ^= acc.l >> 1;
            acc.l ^= acc.l >> 2;
            acc.l ^= acc.l >> 4;
            af.l |= (acc.l & 0x01) ? 0x00 : FLAG_PV;   // ...H.PNC

            tmp.w -= 0x0102;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
