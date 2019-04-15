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

/** Z80IntMode2.h
 *
 * Interrupt mode 2 process (modelled as an instruction!)
 *
 */

bool z80IntMode2()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 2;
            memAddrMode = 0x0000AA9C;
            return false;

        case 1:
            wz.w = (ir.b.h << 8) | d;
            return true;

        case 2:
            return true;

        case 3:
            wz.w = iReg.w;
            acc.w = pc.w;
            oReg.b.l = acc.b.h;
            oReg.b.h = acc.b.l;
            return true;

        case 4:
            return true;

        case 5:
            pc.w = wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
