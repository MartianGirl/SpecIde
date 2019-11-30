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

/** Z80Outi.h
 *
 * Instruction: OUTI
 *
 * Encoding: 11 101 101  10 100 011
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

bool z80Outi()
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
            ++hl.w;
            --bc.b.h;
            wz.w = bc.w;
            oReg.b.l = iReg.b.h;
            return true;

        case 3:
            flg = bc.b.h & (FLAG_S | FLAG_5 | FLAG_3);  // S.5H3PNC
            flg |= (bc.b.h) ? 0x00 : FLAG_Z;            // SZ5H3PNC

            flg |= (iReg.b.h & 0x80) >> 6;              // ......N.
            acc.w = iReg.b.h + hl.b.l;
            flg |= (acc.b.h) ? FLAG_H | FLAG_C : 0x00;  // ...b.H..NC
            acc.w = (acc.w & 0x07) ^ bc.b.h;
            acc.b.l ^= acc.b.l >> 1;
            acc.b.l ^= acc.b.l >> 2;
            acc.b.l ^= acc.b.l >> 4;
            flg |= (acc.b.l & 0x01) ? 0x00 : FLAG_PV;   // ...b.H.PNC
            af.b.l = flg;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
