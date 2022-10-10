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

enum class AccessType {
    CRTC_NA,
    CRTC_RO = 1,
    CRTC_WO,
    CRTC_RW
};

enum class UpdateMode {
    RESET,
    INCREMENT,
    KEEP
};

//#define DEBUGCRTC

class CRTC {
    public:
        CRTC(uint_fast8_t type = 0);

        /** CRTC type. Numbers as in Amstrad CPC community. */
        uint_fast32_t type;
        /** Index of the selected CRTC register. The selected register can
         * be read or written to. */
        uint_fast8_t index;
        /** CRTC registers. */
        uint_fast8_t regs[32];
        /** CRTC bitmasks for CRTC registers. */
        uint_fast8_t mask[32];
        /** Access type for CRTC registers. */
        AccessType dirs[32];

        /** Horizontal total (R0). */
        uint_fast8_t r0_hTotal = 0;
        /** Horizontal displayed (R1). */
        uint_fast8_t r1_hDisplayed = 0;
        /** HSync position (R2). */
        uint_fast8_t r2_hSyncPos = 0;
        /** HSync max width (R3, b3-b0). */
        uint_fast8_t r3l_hSyncWidth = 0;
        /** VSync max width (R3, b7-b4). */
        uint_fast8_t r3h_vSyncWidth = 0;
        /** Vertical total (R4). */
        uint_fast8_t r4_vTotal = 0;
        /** Vertical adjust (R5). */
        uint_fast8_t r5_vAdjust = 0;
        /** Vertical displayed (R6). */
        uint_fast8_t r6_vDisplayed = 0;
        /** VSync position (R7). */
        uint_fast8_t r7_vSyncPos = 0;
        /** Interlace mode. */
        uint_fast8_t r8_interlaceMode = 0;
        /** Raster max address (R9). */
        uint_fast8_t r9_rMaxAddress = 0;

        /** Horizontal counter (C0). */
        uint_fast8_t c0_hCounter = 0;
        /** HSync width counter (C3 Low). */
        uint_fast8_t c3l_hSyncWidth = 0;
        /** VSync width counter (C3H). */
        uint_fast8_t c3h_vSyncWidth = 0;
        /** Vertical counter (C4). */
        uint_fast8_t c4_vCounter = 0;
        /** Adjust counter (C5). */
        uint_fast8_t c5_aCounter = 0;
        /** Raster counter (C9). */
        uint_fast8_t c9_rCounter = 0;

        /** Programmed update for c4_vCounter. */
        UpdateMode c4Update = UpdateMode::KEEP;
        /** Programmed update for c5_aCounter. */
        UpdateMode c5Update = UpdateMode::KEEP;
        /** Programmed update for c9_rCounter. */
        UpdateMode c9Update = UpdateMode::KEEP;
        /** Odd field indicator. */
        bool oddField = true;
        /** Horizontal position where VSync pulse starts. */
        uint_fast8_t vSyncOffset = 0;

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

        /** Updated video offset from R12/R13, not from VMA'. */
        bool videoOffsetUpdated = false;
        /** Raster counter is checked only once per line. */
        bool enableRasterCounter = true;

        bool hDisplay = false;
        bool vDisplay = false;

        bool hSync = false;
        bool vSync = false;
        bool dispEn = false;
        bool vSyncForced = false;
        bool vSyncAuthorized = false;

        /** End of horizontal displayed area. C0 == R1. */
        bool hDispOff = false;
        /** End of vertical displayed area. C4 == R6. */
        bool vDispOff = false;

        /** Processing vertical adjustment lines. */
        bool processVAdjust = false;

        void wrAddress(uint_fast8_t byte);
        void wrRegister(uint_fast8_t byte);
        void rdStatus(uint_fast8_t &byte);
        void rdRegister(uint_fast8_t &byte);
        
        void clock();
        void reset();

        void updateC4();
        void updateC9();
        void updateC5();
        void updateVideoOffset();
};


// vim: et:sw=4:ts=4
