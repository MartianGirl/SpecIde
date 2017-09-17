#pragma once

/** Z80Ini.h
 *
 * Instruction: INI
 *
 * Encoding: 11 101 101  10 100 010
 * M Cycles: 4 (OCF, OCF(5), IOR, MWB)
 * T States: 16
 *
 * Flags:
 * - Let's call k = (HL) + ((C + 1) & FFh)
 * - C, H are set if k > FFh
 * - P is the parity of ((k & 7h) XOR B)
 * - N is bit 7 of (HL)
 * - S, Z, 5 and 3 are set by the result of decrementing B.
 *
 * Notes:
 * - BC register is placed on the bus after B is decremented.
 *
 */

bool z80Ini()
{
    switch (executionStep)
    {
        case 0:
            ioRdCycles = 1;
            memWrCycles = 1;
            memAddrMode = 0x0000002E;
            return false;

        case 1:
            wz.w = bc.w;
            --wz.h;
            bc.w = wz.w;
            return true;

        case 2:
            oReg.l = iReg.h;
            return true;

        case 3:
            ++hl.w;
            af.l = bc.h & (FLAG_S | FLAG_5 | FLAG_3); // S.5.3...
            af.l |= (bc.h) ? 0x00 : FLAG_Z;           // SZ5.3...
            af.l |= (iReg.h & 0x80) >> 6;             // SZ5.3.N.
            acc.w = iReg.h + ((bc.l + 1) & 0xFF);
            af.l |= (acc.h) ? FLAG_H | FLAG_C : 0x00; // SZ5H3.NC
            acc.w = (acc.w & 0x07) ^ bc.h;
            acc.l ^= acc.l >> 1;
            acc.l ^= acc.l >> 2;
            acc.l ^= acc.l >> 4;
            af.l |= (acc.l & 0x01) ? 0x00 : FLAG_PV;   // SZ5H3PNC

            wz.w += 0x0100;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
