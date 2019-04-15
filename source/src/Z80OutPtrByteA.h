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

/** Z80OutPtrByteA.h
 *
 * Instruction: OUT (n), A
 *
 * Encoding: 11 010 011
 * M Cycles: 3 (OCF, MRB, IOWR)
 * T States: 11
 *
 */

bool z80OutPtrByteA()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            ioWrCycles = 1;
            memAddrMode = 0x000000D1;
            return true;

        case 1:
            wz.w = (af.b.h << 8) | iReg.b.h;
            oReg.b.l = af.b.h;
            return true;

        case 2:
            wz.b.h = af.b.h;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
