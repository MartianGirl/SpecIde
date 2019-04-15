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

/** Z80DecPtrHl.h
 *
 * Instruction: DEC (HL)
 *
 */

bool z80DecPtrHl()
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
            // Preserve carry bit.
            flg = af.b.l & FLAG_C;
            flg |= decFlags[iReg.b.h];

            oReg.b.l = iReg.b.h - 1;
            memWrCycles = 1;
            return true;

        case 3:
            af.b.l = flg;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
