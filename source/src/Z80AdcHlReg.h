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
            return true;

        case 1:
            // Save HL and operand.
            wz.w = hl.w;
            acc.w = *regp[p];
            return false;

        case 2:
            // First, do the low byte addition. Carry is in lowest
            // bit of H. Add carry here.
            hl.w = acc.l + wz.l + (af.l & FLAG_C);
            acc.w = acc.h;
            af.l = hl.h & FLAG_C;

            // Perform the addition in H, including low byte carry.
            hl.h = acc.l + wz.h + (af.l & FLAG_C);
            return false;

        case 3:
            // Now check flags:
            // Half carry
            af.l |= (wz.h ^ acc.l ^ hl.h) & FLAG_H;

            // Carry into bit 7
            af.l |= ((wz.h ^ acc.l ^ hl.h) >> 5) & FLAG_PV;
            return false;

        case 4:
            // Carry out of bit 7
            acc.w += wz.h + (af.l & FLAG_C);
            af.l ^= ((acc.h & FLAG_C) << 2) & FLAG_PV;
            af.l &= ~FLAG_C;
            af.l |= (acc.h & FLAG_C);
            return true;

        case 5:
            // Sign is affected by the 16-bit result - hence high byte.
            // 5 and 3 are affected by the high byte.
            af.l |= hl.h & (FLAG_S | FLAG_5 | FLAG_3);
            return false;

        case 6:
            // Zero is affected by the 16-bit result.
            af.l |= (hl.w == 0x0000) ? FLAG_Z : 0x00;
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
