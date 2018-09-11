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

/** Z80LdPtrDeA.h
 *
 * Instruction: LD (DE), A
 *
 * Encoding: 00 010 010
 * M Cycles: 2 (OCF, MWB)
 * T States: 7
 *
 */

bool z80LdPtrDeA()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 1;
            memAddrMode = 0x00000004;

            wz.w = de.w + 1;
            oReg.l = af.h;
            return true;

        case 1:
            wz.h = af.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
