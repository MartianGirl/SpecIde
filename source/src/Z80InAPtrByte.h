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

/** Z80InAPtrByte.h
 *
 * Instruction: IN A, (n)
 *
 * Encoding: 11 011 011
 * M Cycles: 3 (OCF, MRB, IORD)
 * T States: 11
 *
 */

bool z80InAPtrByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            ioRdCycles = 1;
            memAddrMode = 0x000000D1;
            return true;

        case 1:
            wz.w = (af.h << 8) | iReg.h;
            return true;

        case 2:
            af.h = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
