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
        bool gatedMREQ_ = false;

        void selectPen(uint_fast8_t byte);
        void selectColour(uint_fast8_t byte);

        static uint_fast32_t colours[32];

        static uint_fast32_t pixelsX1[0x70000];
        static uint_fast32_t pixelsX2[0x70000];

        /**
         * Gate array sequence.
         *
         * With the information available, looks like this sequence is never reset.
         * In order to be reset, RST needs to be hign, and M1, IORQ and RD need to be
         * low. This last combination never happens in a Z80 CPU.
         */
        static uint_fast8_t constexpr sequence[16] = {
            0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
            0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
        };

        /**
         * 4MHz CPU clock.
         *
         * The equation is (S1 ^ S3) | (S5 ^ S7), so it's true for any value of the
         * sequencer with digits 3, 7, 8 or C.
         * The result is latched, so this table is shifted right one position.
         */
        static bool constexpr phiTable[16] = {
            true, false, false, true, true, false, false, true,
            true, false, false, true, true, false, false, true
        };

        /**
         * 1MHz clock.
         *
         * The equation is !(S2 | S5), so it's true from 0xC0 to 0x03.
         */
        static bool constexpr cClkTable[16] = {
            false, false, false, false, false, false, true, true,
            true, true, true, false, false, false, false, false
        };

        /**
         * Gate Array READY signal.
         *
         * This is the Z80 #WAIT signal.
         */
        static bool constexpr readyTable[16] = {
            true, false, false, false, false, false, false, false,
            false, false, false, false, true, true, true, true
        };
};

// vim: et:sw=4:ts=4
