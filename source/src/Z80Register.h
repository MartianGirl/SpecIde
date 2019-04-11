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

/** Z80Register
 *
 * Each of the 16-bit groups of two 8-bit registers in the Z80.
 *
 */

#include <cstdint>

using namespace std;

struct Z80Pair
{
#if SPECIDE_BYTE_ORDER == 1
    uint8_t h;
    uint8_t l;
#else
    uint8_t l;
    uint8_t h;
#endif
};

union Z80Register
{
    uint16_t w;
    Z80Pair b;
};

/* class Z80Register
{
    public:
        Z80Register();

        void setw(uint_fast16_t value);
        uint_fast16_t getw(void);

        // The ordering of the following variables is important.
        // These will be initialized in the order they are declared here.
        // For the sake of clarity, the constructor relies on this.
        uint_fast16_t reg;

        // Using non-fast pointers for correct bitwise operations.
        uint16_t *pw;
        uint8_t *ph;
        uint8_t *pl;

        uint16_t &w;
        uint8_t &h;
        uint8_t &l;
}; */

// vim: et:sw=4:ts=4
