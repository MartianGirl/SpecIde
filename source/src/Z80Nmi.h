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

/** Z80Nmi.h
 *
 * NMI process (modelled as an instruction!)
 *
 */

bool z80Nmi()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            acc.w = pc.w;
            return false;

        case 1:
            oReg.b.l = acc.b.h;
            oReg.b.h = acc.b.l;
            return true;

        case 2:
            return true;

        case 3:
            pc.w = 0x0066;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
