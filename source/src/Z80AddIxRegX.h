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

/** Z80AddIxRegX.h
 *
 * Instruction: ADD IX, rr
 *
 */

bool z80AddIxRegX()
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
            wz.w = ix.w;
            acc.w = *regpx[p];

            // First, do the low byte addition. Carry is in lowest
            // bit of H. Add carry here.
            ix.w = acc.b.l + wz.b.l;
            acc.w = acc.b.h;
            flg = af.b.l & (FLAG_S | FLAG_Z | FLAG_PV);
            flg |= ix.b.h & FLAG_C;

            // Perform the addition in H, including low byte carry.
            ix.b.h = acc.b.l + wz.b.h + (flg & FLAG_C);

            // Half carry
            flg |= (wz.b.h ^ acc.b.l ^ ix.b.h) & FLAG_H;

            // Carry
            acc.w += wz.b.h + (flg & FLAG_C);
            flg &= ~FLAG_C;
            flg |= (acc.b.h & FLAG_C);

            // 5 and 3 are affected by the high byte.
            flg |= ix.b.h & (FLAG_5 | FLAG_3);
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
