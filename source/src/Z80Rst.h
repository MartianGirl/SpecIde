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

/** Z80Rst.h
 *
 * Instruction: RST 0h
 * Instruction: RST 8h
 * Instruction: RST 10h
 * Instruction: RST 18h
 * Instruction: RST 20h
 * Instruction: RST 28h
 * Instruction: RST 30h
 * Instruction: RST 38h
 *
 */

bool z80Rst()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            return false;

        case 1:
            oReg.l = pc.h;
            oReg.h = pc.l;
            return true;

        case 2:
            return true;

        case 3:
            pc.w = wz.w = (y << 3);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
