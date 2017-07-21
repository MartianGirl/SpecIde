#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>

#include "Z80Defs.h"

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();
        void reset();

        // Constant-ish statics.
        static size_t pixelStart, pixelEnd;
        static size_t hBorderStart, hBorderEnd;
        static size_t hBlankStart, hBlankEnd;
        static size_t hSyncStart, hSyncEnd;

        static size_t scanStart, scanEnd;
        static size_t vBorderStart, vBorderEnd;
        static size_t vBlankStart, vBlankEnd;
        static size_t vSyncStart, vSyncEnd;

        static int_fast32_t tensions[4];
        static int_fast32_t constants[4096];

        static size_t maxScan;
        static size_t maxPixel;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz;
        
        uint_fast16_t z80_a;
        uint_fast16_t z80_c;
        bool cpuClock;
        bool cpuLevel;

        // Video signals
        uint32_t rgba;
        bool hSync, vSync;
        bool hBlank, vBlank;
        bool idle;

        uint32_t colourTable[0x100];

        // Port 0xFE
        uint_fast8_t ioPortIn;
        uint_fast8_t ioPortOut;
        uint_fast8_t tapeIn;

        // Values in EAR/MIC ULA pin (millivolts)
        int_fast32_t c00, c01, c10, c11;

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
