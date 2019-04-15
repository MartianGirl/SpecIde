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

/** Z80LdPtrWordReg.h
 *
 * Instruction: LD (nn), rr
 *
 * Encoding: 11 101 101  01 rr0 011
 * M Cycles: 6 (ED, OCF, MRL, MRH, MWL, MWH)
 * T States: 20
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IY  10
 *  SP  11
 *
 */

bool z80LdPtrWordReg()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00009811;
            return true;

        case 1:
            return true;

        case 2:
            wz.w = iReg.w;
            memWrCycles = 2;
            oReg.w = *(regp[p]);
            return true;

        case 3:
            return true;

        case 4:
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
