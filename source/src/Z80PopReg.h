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

/** Z80PopReg.h
 *
 * Instruction: POP rr
 *
 * Encoding: 11 rr0 001
 * M Cycles: 3 (OCF, MRL, MRH)
 * T States: 10
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  HL  10
 *  AF  11
 *
 */

bool z80PopReg()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x000000BB;
            return true;

        case 1:
            return true;

        case 2:
            *(regp2[p]) = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
