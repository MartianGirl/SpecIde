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

/** Z80LdRegYWord.h
 *
 * FD Prefixed instructions.
 * Instruction: LD BC, nn
 * Instruction: LD DE, nn
 * Instruction: LD IY, nn
 * Instruction: LD SP, nn
 *
 * Encoding: 11 111 101  00 rr0 001
 * M Cycles: 4 (FD, OCF, MRL, MRH)
 * T States: 14
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  IY  10
 *  SP  11
 *
 */

bool z80LdRegYWord()
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
            *regpy[p] = iReg.w;
            flg = 0;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
