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

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>
#include <cstddef>

#include "SoundDefs.h"
#include "Z80Defs.h"

using namespace std;

uint_fast32_t constexpr ENDD = 4;
uint_fast32_t constexpr ENDS = 3;
uint_fast32_t constexpr DUPL = 2;
uint_fast32_t constexpr SNOW = 1;
uint_fast32_t constexpr NONE = 0;

uint_fast32_t constexpr ULA_48KISS2 = 0;
uint_fast32_t constexpr ULA_48KISS3 = 1;
uint_fast32_t constexpr ULA_128K = 2;
uint_fast32_t constexpr ULA_PLUS2 = 3;
uint_fast32_t constexpr ULA_PLUS3 = 4;
uint_fast32_t constexpr ULA_PENTAGON = 5;
uint_fast32_t constexpr NUM_MODELS = 6;

uint_fast32_t constexpr NUM_CHECKPOINTS = 6;

class ULA {

    public:
        ULA();

        void clock();
        void reset();
        void paint();

        void generateVideoControlSignals();
        void generateInterrupt();
        void tapeEarMic();
        void setEarLevel(bool level, bool playing);
        uint_fast8_t ioRead();
        void ioWrite(uint_fast8_t byte);
        void beeper();
        int sample();
        void start();
        void updateAttributes();

        void generateVideoDataUla();
        void generateVideoDataGa();
        void generateVideoDataPentagon();

        void setUlaVersion(uint_fast8_t version);

        uint32_t average(uint32_t *ptr);

        uint_fast16_t vBorderStart = 0x0C0;
        uint_fast16_t vBlankStart = 0x0F8;
        uint_fast16_t vBlankEnd = 0x0FF;
        uint_fast16_t vSyncStart = 0x0F8;
        uint_fast16_t vSyncEnd = 0x0FC;
        uint_fast16_t maxScan = 0x138;

        uint_fast16_t paintPixel = 0x004;

        static float constexpr voltages[6][4] = {
            {0.391, 0.728, 3.653, 3.790}, // ULA 5C (Issue 2)
            {0.342, 0.652, 3.591, 3.753}, // ULA 6C (Issue 3)
            {0.342, 0.652, 3.591, 3.753}, // ULA 7K (128K)
            {0.342, 0.652, 3.591, 3.753}, // GA 40056 (Plus2)
            {0.342, 0.652, 3.591, 3.753}, // GA 40077 (Plus2A/Plus2B/Plus3)
            {0.342, 0.652, 3.591, 3.753}  // Pentagon
        };

        float vEnd = 0.0;
        float vInc = 0.0;
        float vCap = 0.0;

        static bool delayTable[16];
        static bool idleTable[16];
        static bool memTable[16];
        static uint_fast32_t snowTable[16];

        static uint32_t colourTable[0x100];
        uint32_t colour[2];
        static uint8_t averageTable[0x100][0x100];
        static uint_fast32_t constexpr X_SIZE = 360;
        static uint_fast32_t constexpr Y_SIZE = 625;
        static uint32_t pixelsX1[X_SIZE * Y_SIZE / 2];
        static uint32_t pixelsX2[X_SIZE * Y_SIZE];
        uint_fast32_t xPos = 0;
        uint_fast32_t yPos = 0;
        uint_fast32_t yInc = 1;
        uint_fast32_t frame = 0;

        // These values depend on the model
        uint_fast8_t ulaVersion = 0;
        void (ULA::*generateVideoData)() = &ULA::generateVideoDataUla;

        uint_fast16_t checkPoint = 0;
        static uint_fast16_t constexpr checkPointValues[NUM_MODELS][6] {
            // videoStart, hBorderStart, videoEnd, hBlankStart, hBlankEnd, maxPixel
            { 0x008, 0x100, 0x108, 0x140, 0x19F, 0x1C0 }, // 48K Issue 2
            { 0x008, 0x100, 0x108, 0x140, 0x19F, 0x1C0 }, // 48K Issue 3
            { 0x008, 0x100, 0x108, 0x140, 0x19F, 0x1C8 }, // 128K Toastrack
            { 0x008, 0x100, 0x108, 0x140, 0x19F, 0x1C8 }, // Plus2
            { 0x008, 0x104, 0x108, 0x140, 0x19F, 0x1C8 }, // Plus2A/Plus3
            { 0x008, 0x100, 0x108, 0x138, 0x198, 0x1C0 }  // Pentagon
        };

        // ULA internals
        uint_fast16_t pixel = 0;
        uint_fast16_t scan = 0;
        uint_fast8_t flash = 0;
        bool z80Clock = false;

        uint_fast16_t z80_c_1 = 0xFFFF;
        uint_fast16_t z80_c_2 = 0xFFFF;

        uint_fast16_t dataAddr;
        uint_fast16_t attrAddr;

        uint_fast8_t data;
        uint_fast8_t attr;
        uint_fast8_t dataReg;
        uint_fast8_t attrReg;
        uint_fast8_t latch;

        double ear = 0;

        // Audio and tape signals
        int filter[FILTER_BZZ_SIZE];
        size_t index = 0;
        bool playSound = true;
        bool tapeSound = true;
        uint_fast8_t micMask = 0x03;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;

        uint_fast16_t z80_a = 0xFFFF;
        uint_fast16_t z80_c = 0xFFFF;
        bool contendedBank = false;
        bool cpuClock = true;
        bool ulaReset = true;
        uint_fast32_t snow = NONE;

        // Video signals
        uint32_t rgba;
        bool video = true;
        bool border = false;
        bool idle = true;
        bool mem = true;

        // Useful video signals
        bool vSync = false;
        bool blanking = false;

        // Port 0xFE
        uint_fast8_t soundBits = 0x00;
        uint_fast8_t inMask = 0xBF;
        uint_fast8_t borderAttr = 0x00;
        bool tapeLevel = true;
        bool tapePlaying = false;

        // Keyboard polling
        uint_fast8_t keys[8];
        uint_fast8_t keyData[8];

        // ULA interrupt
        uint_fast16_t c;
        uint_fast16_t interruptStart;
        uint_fast16_t interruptEnd;

        // Scanline modes.
        uint_fast32_t scanlines = 0;
};

// vim: et:sw=4:ts=4
