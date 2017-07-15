#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>
#include <iostream>

#include "Z80Defs.h"

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz;
        
        uint_fast16_t z80_a;
        uint_fast16_t z80_c;
        bool cpuClock;

        // Video signals
        size_t maxScan;
        size_t maxPixel;

        uint32_t rgba;
        bool hSync, vSync;
        bool hBlank, vBlank;

        size_t pixelStart, pixelEnd;
        size_t hBorderStart, hBorderEnd;
        size_t hBlankStart, hBlankEnd;
        size_t hSyncStart, hSyncEnd;

        size_t scanStart, scanEnd;
        size_t vBorderStart, vBorderEnd;
        size_t vBlankStart, vBlankEnd;
        size_t vSyncStart, vSyncEnd;

        uint32_t colourTable[0x100];

        // Port 0xFE
        uint_fast8_t ioPortIn;
        uint_fast8_t ioPortOut;
        uint_fast8_t tapeIn;

        // Values in EAR/MIC ULA pin (millivolts)
        int_fast32_t c00, c01, c10, c11;
        int_fast32_t tensions[4];
        int_fast32_t constants[4096];

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
