/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
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

#include <cstdint>
#include <cstddef>

enum class AccessType
{
    CRTC_NA,
    CRTC_RO = 1,
    CRTC_WO,
    CRTC_RW
};

class CRTC {
    public:
        CRTC(uint_fast8_t type = 1);

        uint_fast32_t type;
        uint_fast8_t index;
        uint_fast8_t regs[32];
        uint_fast8_t mask[32];
        AccessType dirs[32];

        /** Horizontal counter. */
        uint_fast8_t hCounter = 0;
        /** Horizontal total (R0). */
        uint_fast8_t hTotal = 0;
        /** Horizontal displayed (R1). */
        uint_fast8_t hDisplayed = 0;
        /** HSync position (R2). */
        uint_fast8_t hsPos = 0;

        /** HSync width counter. */
        uint_fast8_t hswCounter = 0;
        /** HSync max width (R3, b3-b0). */
        uint_fast8_t hswMax = 0;

        /** Vertical counter. */
        uint_fast8_t vCounter = 0;
        /** Vertical total (R4). */
        uint_fast8_t vTotal = 0;
        /** Vertical adjust (R5). */
        uint_fast8_t vAdjust = 0;
        /** Vertical displayed (R6). */
        uint_fast8_t vDisplayed = 0;

        /** VSync position (R7). */
        uint_fast8_t vsPos = 0;
        /** VSync width counter. */
        uint_fast8_t vswCounter = 0;
        /** VSync max width (R3, b7-b4). */
        uint_fast8_t vswMax = 0;

        /** Raster counter. */
        uint_fast8_t rCounter = 0;
        /** Raster max address (R9). */
        uint_fast8_t rMax = 0;

        bool hh;

        uint_fast8_t status = 0;

        /** Address of current character line. */
        uint_fast16_t lineAddress = 0;
        /** Address of current screen character. */
        uint_fast16_t charAddress = 0;
        /** Address of current scan. */
        uint_fast16_t byteAddress = 0;
        /** Address of current mem page. */
        uint_fast16_t pageAddress = 0;

        bool hDisplay = false;
        bool vDisplay = false;

        bool hSync = false;
        bool vSync = false;
        bool dispEn = false;

        void wrAddress(uint_fast8_t byte);
        void wrRegister(uint_fast8_t byte);
        void rdStatus(uint_fast8_t &byte);
        void rdRegister(uint_fast8_t &byte);
        
        void clock();
};


// vim: et:sw=4:ts=4
