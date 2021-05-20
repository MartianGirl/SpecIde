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

#include "CRTC.h"

class GateArray {

    public:
        /** CRTC instance. */
        CRTC crtc;

        /** Selected pen. */
        uint_fast8_t pen;
        /** Pen colour references. */
        uint_fast8_t pens[16];
        /** Border colour reference. */
        uint_fast8_t border;
        /** New video mode. */
        uint_fast8_t newMode;
        /** Actual video mode. */
        uint_fast8_t actMode;
        /** Lower ROM readable at $0000-$3FFF. */
        bool lowerRom;
        /** Upper ROM readable at $C000-$FFFF. */
        bool upperRom;

        /** Cycle counter. */
        uint_fast32_t counter = 0;
        /** HSYNC counter for INT generation. */
        uint_fast32_t intCounter = 0;
        /** CCLK rising edges happen at step 6. */
        static uint_fast32_t constexpr CCLK_RISE = 0x06;
        /** CCLK falling edges happen at step 11. */
        static uint_fast32_t constexpr CCLK_FALL = 0x0b;

        bool cpuClock() const { return cpuTable[counter]; }
        bool cpuReady() const { return readyTable[counter]; }
        bool cClk() const { return cClkTable[counter]; }
        bool cClkRise() const { return counter == CCLK_RISE; }
        bool cClkFall() const { return counter == CCLK_FALL; }
        bool muxVideo() const { return muxTable[counter]; }

        /**
         * Write a byte to the Gate Array.
         *
         * This function invokes the appropriate action.
         *
         * @param byte The byte from data bus.
         */
        void write(uint_fast8_t byte);

        /**
         * Select a pen on which to operate.
         *
         * @param byte Byte from Z80.
         */
        void selectPen(uint_fast8_t byte);

        /**
         * Select colour for currently selected pen.
         *
         * @param byte Byte from Z80.
         */
        void selectColour(uint_fast8_t byte);

        /**
         * Select screen mode, ROM visibility, and delay interrupts.
         *
         * @param byte Byte from Z80.
         */
        void selectScreenAndRom(uint_fast8_t byte);

        static uint_fast32_t constexpr X_SIZE = 896;
        static uint_fast32_t constexpr Y_SIZE = 625;

        static uint_fast32_t pixelsX1[X_SIZE * Y_SIZE / 2];
        static uint_fast32_t pixelsX2[X_SIZE * Y_SIZE];

        /** Averaged colours between two scans. */
        uint32_t averagedColours[1024];
        /** Colour definitions. */
#if SPECIDE_BYTE_ORDER == 1
        static uint32_t constexpr colours[32] = {
            0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF,
            0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
            0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF,
            0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
            0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF,
            0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
            0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF,
            0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF};
#else
        static uint32_t constexpr colours[32] = {
            0xFF7F7F7F, 0xFF7F7F7F, 0xFF7FFF00, 0xFF7FFFFF,
            0xFF7F0000, 0xFF7F00FF, 0xFF7F7F00, 0xFF7F7FFF,
            0xFF7F00FF, 0xFF7FFFFF, 0xFF00FFFF, 0xFFFFFFFF,
            0xFF0000FF, 0xFFFF00FF, 0xFF007FFF, 0xFFFF7FFF,
            0xFF7F0000, 0xFF7FFF00, 0xFF00FF00, 0xFFFFFF00,
            0xFF000000, 0xFFFF0000, 0xFF007F00, 0xFFFF7F00,
            0xFF7F007F, 0xFF7FFF7F, 0xFF00FF7F, 0xFFFFFF7F,
            0xFF00007F, 0xFFFF007F, 0xFF007F7F, 0xFFFF7F7F};
#endif

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
         * 4MHz CPU clock edge.
         *
         * Here the CPU clock changes phase. This is useful for clocking Z80::clock()
         */
        static bool constexpr cpuTable[16] = {
            false, true, false, true, false, true, false, true,
            false, true, false, true, false, true, false, true
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

        /**
         * Address MUX setting table.
         *
         * Select Video address (true) or CPU address (false) to read data.
         */
        static bool constexpr muxTable[16] = {
            true, true, true, true, true, true, true, true,
            true, true, false, false, false, false, false, false
        };

        static bool constexpr mode0Table[16] = {
            COLOUR_KEEP, COLOUR_KEEP, COLOUR_KEEP, COLOUR_MOVE,
            COLOUR_KEEP, COLOUR_KEEP, COLOUR_KEEP, COLOUR_LOAD,
            COLOUR_KEEP, COLOUR_KEEP, COLOUR_KEEP, COLOUR_MOVE,
            COLOUR_KEEP, COLOUR_KEEP, COLOUR_KEEP, COLOUR_LOAD
        };

        static bool constexpr mode1Table[16] = {
            COLOUR_KEEP, COLOUR_MOVE, COLOUR_KEEP, COLOUR_MOVE,
            COLOUR_KEEP, COLOUR_MOVE, COLOUR_KEEP, COLOUR_LOAD,
            COLOUR_KEEP, COLOUR_MOVE, COLOUR_KEEP, COLOUR_MOVE,
            COLOUR_KEEP, COLOUR_MOVE, COLOUR_KEEP, COLOUR_LOAD
        };

        static bool constexpr mode2Table[16] = {
            COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE,
            COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE, COLOUR_LOAD,
            COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE,
            COLOUR_MOVE, COLOUR_MOVE, COLOUR_MOVE, COLOUR_LOAD
        };
};
// vim: et:sw=4:ts=4
