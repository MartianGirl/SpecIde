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

#include "Z80Register.h"

// This works because the member variables were declared in this same order
// in the header file.
Z80Register::Z80Register() :
    reg(0x0000),
    pw(reinterpret_cast<uint16_t*>(&reg)),
#if SPECIDE_BYTE_ORDER == 1
    ph(reinterpret_cast<uint8_t*>(pw) + sizeof(uint_fast16_t) - 2),
    pl(reinterpret_cast<uint8_t*>(pw) + sizeof(uint_fast16_t) - 1),
#else
    ph(reinterpret_cast<uint8_t*>(pw) + 1),
    pl(reinterpret_cast<uint8_t*>(pw) + 0),
#endif 
    w(*pw),
    h(*ph),
    l(*pl)
{
}

void Z80Register::setw(uint_fast16_t value)
{
    reg = value;
}

uint_fast16_t Z80Register::getw()
{
    return reg;
}

// vim: et:sw=4:ts=4
