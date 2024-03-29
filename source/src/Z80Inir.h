/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/** Z80Inir.h
 *
 * Instruction: INIR
 *
 * Encoding: 11 101 101  10 110 010
 * M Cycles: 5 (OCF, OCF(5), IOR, MWB, CPU(5)) if B != 00h
 *           4 (OCF, OCF(5), IOR, MWB)         if B == 00h
 * T States: 21 if B != 00h
 *           16 if B == 00h
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

bool z80Inir()
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
            return true;

        case 2:
            oReg.b.l = iReg.b.h;
            return true;

        case 3:
            ++hl.w;
            --bc.b.h;
            flg = bc.b.h & (FLAG_S | FLAG_5 | FLAG_3); // S.5.3...
            flg |= (bc.b.h) ? 0x00 : FLAG_Z;           // SZ5.3...
            flg |= (iReg.b.h & 0x80) >> 6;              // SZ5.3.N.
            acc.w = iReg.b.h + ((bc.b.l + 1) & 0xFF);
            flg |= (acc.b.h) ? FLAG_H | FLAG_C : 0x00; // SZ5H3.NC
            acc.w = (acc.w & 0x07) ^ bc.b.h;
            acc.b.l ^= acc.b.l >> 1;
            acc.b.l ^= acc.b.l >> 2;
            acc.b.l ^= acc.b.l >> 4;
            flg |= (acc.b.l & 0x01) ? 0x00 : FLAG_PV;   // SZ5H3PNC
            af.b.l = flg;

            if (bc.b.h != 0x00)
            {
                cpuProcCycles = 1;
                skipCycles = 4;
            }
            else
                prefix = PREFIX_NO;
            return true;

        case 4:
        case 5:
        case 6:
        case 7:
            return false;

        case 8:
            wz.w = pc.w - 1;
            pc.w -= 2;
            flg = af.b.l & ~(FLAG_5 | FLAG_3);
            flg |= pc.b.h & (FLAG_5 | FLAG_3);

            acc.b.l = bc.b.h;
            if (af.b.l & FLAG_C) {
                acc.b.l += (af.b.l & FLAG_N) ? -1 : 1;
                flg = (flg & ~FLAG_H) | ((bc.b.h ^ acc.b.l) & FLAG_H);
            }
            acc.b.l &= 0x07;
            acc.b.l ^= acc.b.l >> 1;
            acc.b.l ^= acc.b.l >> 2;
            acc.b.l ^= acc.b.l >> 4;
            flg ^= (acc.b.l & 1) ? FLAG_PV : 0;

            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
