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

/** Z80AdcHlReg.h
 *
 * Instruction: ADC HL, rr
 *
 */

bool z80AdcHlReg()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            cpuProcCycles = 2;
            skipCycles = 3;
            return true;

        case 1:
        case 2:
        case 3:
            return false;

        case 4:
            // Save HL and operand.
            wz.w = hl.w;
            acc.w = *regp[p];

            // First, do the low byte addition. Carry is in lowest
            // bit of H. Add carry here.
            hl.w = acc.b.l + wz.b.l + (af.b.l & FLAG_C);
            acc.w = acc.b.h;
            flg = hl.b.h & FLAG_C;

            // Perform the addition in H, including low byte carry.
            hl.b.h = acc.b.l + wz.b.h + (flg & FLAG_C);

            // Now check flags:
            // Half carry
            flg |= (wz.b.h ^ acc.b.l ^ hl.b.h) & FLAG_H;

            // Carry into bit 7
            flg |= ((wz.b.h ^ acc.b.l ^ hl.b.h) >> 5) & FLAG_PV;

            // Carry out of bit 7
            acc.w += wz.b.h + (flg & FLAG_C);
            flg ^= ((acc.b.h & FLAG_C) << 2) & FLAG_PV;
            flg &= ~FLAG_C;
            flg |= (acc.b.h & FLAG_C);

            // Sign is affected by the 16-bit result - hence high byte.
            // 5 and 3 are affected by the high byte.
            flg |= hl.b.h & (FLAG_S | FLAG_5 | FLAG_3);

            // Zero is affected by the 16-bit result.
            flg |= (hl.w == 0x0000) ? FLAG_Z : 0x00;
            af.b.l = flg;

            skipCycles = 2;
            return true;

        case 5:
        case 6:
            return false;

        case 7:
            ++wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
