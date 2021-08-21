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
        CRTC(uint_fast8_t type = 0);

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

        /** Adjust counter. */
        uint_fast8_t vaCounter = 0;
        /** Vertical adjust (R5). */
        uint_fast8_t vAdjust = 0;

        /** Interlace mode. */
        uint_fast8_t interlace = 0;
        /** Odd field indicator. */
        bool oddField = true;
        /** Horizontal position where VSync pulse starts. */
        uint_fast8_t vSyncOffset = 0;

        /** Programmed number of scans in the CRTC. */
        uint_fast32_t maxScans = 312;
        /** Separation between VSYNCs for max. 72Hz VFreq. */
        uint_fast32_t vSyncSeparation = 300;

        /** Address of current character line. */
        uint_fast16_t lineAddress = 0;
        /** Address of current screen character. */
        uint_fast16_t charAddress = 0;
        /** Address of current scan. */
        uint_fast16_t byteAddress = 0;
        /** Address of current mem page. */
        uint_fast16_t pageAddress = 0;
        bool updateLineAddress = false;

        bool hDisplay = false;
        bool vDisplay = false;

        bool hSync = false;
        bool vSync = false;
        bool dispEn = false;

        /** End of scan. Usually when C0 == R0. */
        bool endOfScan = false;
        /** End of character row. This condition is triggered when C9 == R9 + 1. */
        bool endOfRow = false;
        /** Screen frame has ended. */
        bool endOfFrame = false;
        /** First scan of character row. Usually this means C9 == 0. */
        bool firstScanInRow = false;
        /** Last scan of character row. Usually this means C9 == R9. */
        bool lastScanInRow = false;
        /** First character row on the screen. */
        bool firstRow = false;
        /** Last character row on the screen. */
        bool lastRow = false;
        /** End of horizontal displayed area. C0 == R1. */
        bool hDispOff = false;
        /** End of vertical displayed area. C4 == R6. */
        bool vDispOff = false;

        bool finishFrameAttempt = false;
        bool enterVAdjust = false;
        bool finishRow = false;
        bool processVAdjust = false;
        bool finishFrame = false;
        uint_fast8_t nextVCounter = 0;
        uint_fast8_t nextRCounter = 0;
        uint_fast8_t nextACounter = 0;

        bool vTotalUpdated = false;
        bool rMaxUpdated = false;

        void wrAddress(uint_fast8_t byte);
        void wrRegister(uint_fast8_t byte);
        void rdStatus(uint_fast8_t &byte);
        void rdRegister(uint_fast8_t &byte);
        
        void clock();
        void reset();
};


// vim: et:sw=4:ts=4
