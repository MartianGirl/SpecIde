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

/** Memory
 *
 * Memory bank.
 *
 * This is intended to be a TMS4116-like memory. The pinout is:
 * 1. VBB (-5V)                     16. VSS (GND)
 * 2. D                             15. CAS# (Column Address Strobe)
 * 3. W#                            14. Q
 * 4. RAS# (Row Address Strobe)     13. A6
 * 5. A0                            12. A3
 * 6. A2                            11. A4
 * 7. A1                            10. A5
 * 8. VDD (12V)                      9. VCC (5V)
 * 
 * The point here is, the memories are asynchronous. The addresses are
 * latched by the CAS#/RAS# signals.
 *
 */

#include <cstddef>
#include <cstdint>
#include <vector>

using namespace std;

class Memory
{
    public:
        Memory(size_t addrWidth, bool readOnly = false);

        /** Issue a clock tick.
         */
        void clock();
        uint_fast8_t read(uint_fast16_t addr);
        void write(uint_fast16_t addr, uint_fast8_t data);

        uint_fast32_t size;
        uint_fast32_t mask;

        vector<uint_fast8_t> memory;

        bool rom;
        bool wr_;
        bool rd_;
        bool as_;   // Address strobe

        uint_fast16_t a;
        uint_fast8_t d;
};

// vim: et:sw=4:ts=4
