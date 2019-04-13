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

/** Z80BitNPtrHl.h
 *
 * Instruction: BIT n, (HL)
 *
 * Encoding: 11 001 011  01 bbb 110
 * M Cycles: 2 (OCF, OCF, MRB(4))
 * T States: 12
 *
 */

bool z80BitNPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            acc.b.l = iReg.b.h;
            acc.b.l &= (1 << y);

            flg = af.b.l & FLAG_C;
            flg |= FLAG_H;
            flg |= acc.b.l & FLAG_S;
            flg |= wz.b.h & (FLAG_5 | FLAG_3);
            flg |= (acc.b.l) ? 0x00 : (FLAG_Z | FLAG_PV);
            return false;

        case 2:
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
