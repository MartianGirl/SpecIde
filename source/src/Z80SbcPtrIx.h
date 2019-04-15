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

/** Z80SbcPtrIx.h
 *
 * Instruction: SBC A, (IX+d)
 *
 */

bool z80SbcPtrIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000061;
            return true;

        case 1:
            cpuProcCycles = 1;
            return true;

        case 2:
            wz.b.l = iReg.b.h;
            return false;

        case 3:
            wz.b.h = ((wz.b.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 4:
            wz.w += ix.w;
            return false;

        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
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
