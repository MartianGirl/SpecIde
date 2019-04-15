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

/** Z80SbcPtrHl.h
 *
 * Instruction: SBC A, (HL)
 *
 */

bool z80SbcPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            acc.w = af.b.h - iReg.b.h;
            acc.w -= af.b.l & FLAG_C;

            flg = acc.b.l & (FLAG_S | FLAG_5 | FLAG_3);
            flg |= FLAG_N;
            flg |= (acc.b.l ^ iReg.b.h ^ af.b.h) & FLAG_H;
            flg |= (((acc.b.l ^ iReg.b.h ^ af.b.h) >> 5) 
                    ^ (acc.b.h << 2)) & FLAG_PV;
            flg |= acc.b.h & FLAG_C;                   // S.5H3V0C
            flg |= (acc.b.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
            af.b.h = acc.b.l;
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
