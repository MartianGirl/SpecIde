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

/** Z80IncRegP.h
 *
 * Instruction: INC rr
 *
 * Encoding: 00 rr0 011
 * M Cycles: 1 (OCF(6))
 * T States: 6
 *
 *  Reg   rr
 *  ---- ----
 *   BC   00
 *   DE   01
 *   HL   10
 *   SP   11
 *
 */

bool z80IncRegP()
{
    switch (executionStep)
    {
        case 0:
        case 1:
            return false;

        case 2:
            ++*regp[p];
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
