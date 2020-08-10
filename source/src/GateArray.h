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

class GateArray
{
    public:
        void write(uint_fast8_t byte);
        uint_fast8_t read();

        uint_fast8_t pen;
        uint_fast8_t pens[16];
        uint_fast8_t border;

        uint_fast8_t newMode;
        uint_fast8_t actMode;

        bool lowerRom;
        bool upperRom;
        bool interrupt;
        uint_fast8_t ram;

        size_t counter;

        bool cClk = false;
        bool cClkEdge = false;

        void selectPen(uint_fast8_t byte);
        void selectColour(uint_fast8_t byte);

        static uint_fast32_t colours[32];

        static uint_fast32_t pixelsX1[0x70000];
        static uint_fast32_t pixelsX2[0x70000];

        static uint_fast8_t sequence[16];
};

// vim: et:sw=4:ts=4
