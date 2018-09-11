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

/** Z80RetI.h
 *
 * Instruction: RETI
 * Instruction: RETN
 *
 */

bool z80RetI()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 0;
            memAddrMode = 0x000000BB;
            return true;

        case 1:
            return true;

        case 2:
            iff = ((iff & IFF2) >> 2) | (iff & IFF2);
            pc.w = wz.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
