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

        uint_fast8_t type;
        uint_fast8_t index;
        uint_fast8_t regs[32];
        AccessType dirs[32];

        CRTC(uint_fast8_t type = 1);

        uint_fast8_t hCounter = 0;
        uint_fast8_t hswCounter = 0;
        uint_fast8_t hswMax = 14;

        uint_fast8_t vCounter = 0;
        uint_fast8_t vswCounter = 0;
        uint_fast8_t vswMax = 8;

        uint_fast8_t rCounter = 0;
        bool hh;

        /** Address of next accessed byte. */
        uint_fast16_t byteAddress = 0;
        /** Address of current scan. */
        uint_fast16_t scanAddress = 0;
        /** Address of current character line. */
        uint_fast16_t lineAddress = 0;
        /** Address of current mem page. */
        uint_fast16_t pageAddress = 0;

        bool hDisplay = false;
        bool vDisplay = false;

        bool hSync = false;
        bool vSync = false;
        bool dispEn = false;
        bool hSyncFall = false;
        bool vSyncRise = false;

        void wrAddress(uint_fast8_t byte);
        void wrRegister(uint_fast8_t byte);
        uint_fast8_t rdStatus();
        uint_fast8_t rdRegister();
        
        void clock();
};


// vim: et:sw=4:ts=4
