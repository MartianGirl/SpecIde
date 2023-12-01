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

/** Z80Cpir.h
 *
 * Instruction: CPIR
 *
 * Encoding: 11 101 101  10 110 001
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

bool z80Cpir()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            skipCycles = 4;
            return true;

        case 2:
        case 3:
        case 4:
        case 5:
            return false;

        case 6:
            --bc.w;
            ++hl.w;
            acc.b.l = af.b.h - iReg.b.h;
            flg = af.b.l & FLAG_C;                          // 0000000C
            flg |= FLAG_N;                                  // 000000NC
            flg |= 
                (af.b.h ^ iReg.b.h ^ acc.b.l) & FLAG_H;     // 000H00NC
            flg |= acc.b.l & FLAG_S;                        // S00H00NC
            flg |= (acc.b.l) ? 0x00 : FLAG_Z;               // SZ0H00NC
            acc.b.l -= (flg & FLAG_H) >> 4;
            flg |= (acc.b.l & FLAG_3);                      // SZ0H30NC
            flg |= (acc.b.l << 4) & FLAG_5;                 // SZ5H30NC
            flg |= (bc.w) ? FLAG_PV : 0x00;                 // SZ5H3PNC
            af.b.l = flg;

            ++wz.w;
            if (bc.w != 0x0000 && ((flg & FLAG_Z) == 0x00))
            {
                cpuProcCycles = 1;
                skipCycles = 4;
            }
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
            pc.w -= 2;
            flg = af.b.l & ~(FLAG_5 | FLAG_3);
            flg |= pc.b.h & (FLAG_5 | FLAG_3);
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
