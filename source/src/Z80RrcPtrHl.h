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

/** Z80RrcPtrHl.h
 *
 * Instruction: RRC (HL)
 *
 * Encoding: 11 001 011  00 001 110
 * M Cycles: 4 (OCF, OCF, MRB(4), MWB)
 * T States: 15
 *
 * Flags: SZ503P0C
 * - LSB is copied into MSB and into CF.
 *
 */

bool z80RrcPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000022;
            skipCycles = 1;
            return true;

        case 1:
            return false;

        case 2:
            af.b.l = flg = rrcFlags[iReg.b.h];

            acc.b.l = iReg.b.h;
            acc.b.h = acc.b.l & 0x01;
            acc.w >>= 1;

            oReg.b.l = acc.b.l;
            memWrCycles = 1;
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
