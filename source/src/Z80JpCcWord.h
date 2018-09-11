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

/** Z80JpCcWord.h
 *
 * JP NZ, nn Instruction.
 * JP Z, nn Instruction.
 * JP NC, nn Instruction.
 * JP C, nn Instruction.
 * JP PO, nn Instruction.
 * JP PE, nn Instruction.
 * JP P, nn Instruction.
 * JP M, nn Instruction.
 *
 */

bool z80JpCcWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000011;
            return true;

        case 1:
            return true;

        case 2:
            wz.w = iReg.w;
            switch (y)
            {
                case 0:
                    if ((af.l & FLAG_Z) == 0x00)
                        pc.w = wz.w;
                    break;
                case 1:
                    if ((af.l & FLAG_Z) == FLAG_Z)
                        pc.w = wz.w;
                    break;
                case 2:
                    if ((af.l & FLAG_C) == 0x00)
                        pc.w = wz.w;
                    break;
                case 3:
                    if ((af.l & FLAG_C) == FLAG_C)
                        pc.w = wz.w;
                    break;
                case 4:
                    if ((af.l & FLAG_PV) == 0x00)
                        pc.w = wz.w;
                    break;
                case 5: 
                    if ((af.l & FLAG_PV) == FLAG_PV)
                        pc.w = wz.w;
                    break;
                case 6:
                    if ((af.l & FLAG_S) == 0x00)
                        pc.w = wz.w;
                    break;
                case 7:
                    if ((af.l & FLAG_S) == FLAG_S)
                        pc.w = wz.w;
                    break;
                default: assert(false);
            }
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
