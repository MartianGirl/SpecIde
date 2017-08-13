#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>
#include <cstddef>

#include "Z80Defs.h"

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();
        void reset();

        // Constant-ish statics.
        static constexpr size_t pixelStart = 0x000;
        static constexpr size_t pixelEnd = 0x0FF;
        static constexpr size_t hBorderStart = 0x100;
        static constexpr size_t hBorderEnd = 0x1BF;
        static constexpr size_t hBlankStart = 0x140;
        static constexpr size_t hBlankEnd = 0x19F;
        static constexpr size_t hSyncStart = 0x150;
        static constexpr size_t hSyncEnd = 0x16F;

        static constexpr size_t scanStart = 0x000;
        static constexpr size_t scanEnd = 0x0BF;
        static constexpr size_t vBorderStart = 0x0C0;
        static constexpr size_t vBorderEnd = 0x137;
        static constexpr size_t vBlankStart = 0x0F8;
        static constexpr size_t vBlankEnd = 0x0FF;
        static constexpr size_t vSyncStart = 0x0F8;
        static constexpr size_t vSyncEnd = 0x0FB;

        static int_fast32_t tensions[4];
        static int_fast32_t constants[4096];

        static constexpr size_t maxScan = 312;
        static constexpr size_t maxPixel = 448;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz;
        
        uint_fast16_t z80_a;
        uint_fast16_t z80_c;
        bool cpuClock;
        bool ulaReset;

        // Video signals
        uint32_t rgba;
        bool hSync, vSync;
        bool hBlank, vBlank;
        bool display;
        bool idle;

        uint32_t colourTable[0x100];

        // Port 0xFE
        uint_fast8_t ioPortIn;
        uint_fast8_t ioPortOut;
        uint_fast8_t borderAttr;
        uint_fast8_t tapeIn;

        // Values in EAR/MIC ULA pin (millivolts)
        int_fast32_t c00, c01, c10, c11;

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
