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

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz;
        
        uint_fast16_t z80_a;
        uint_fast16_t z80_c, z80_c_delayed;
        bool contentionWindow;
        bool memContention;
        bool ioContention;
        bool cpuWait, cpuClock;

        // Video signals
        size_t scan, maxScan;
        size_t pixel, maxPixel;

        uint_fast8_t flash;
        uint_fast32_t dataReg, attrReg;
        uint8_t &data, &attr;
        uint8_t &dataLatch, &attrLatch;
        uint_fast16_t dataAddr, attrAddr;

        uint32_t rgba;
        bool hSync, vSync;
        bool hBlank, vBlank;
        bool display;

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
        int_fast32_t capacitor;
        uint_fast8_t tapeIn;

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
