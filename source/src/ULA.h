#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>

#include "Z80Defs.h"

#include <SFML/Window.hpp>

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();

        // Memory signals
        uint16_t a;
        uint8_t d;
        bool hiz;
        
        uint16_t z80_a;
        uint16_t z80_c, z80_c_delayed;
        bool contentionWindow;
        bool memContention;
        bool ioContention;
        bool cpuWait, cpuClock;

        // Video signals
        size_t scan, maxScan;
        size_t pixel, maxPixel;

        uint8_t flash;
        uint32_t dataReg, attrReg;
        uint8_t &data, &attr;
        uint8_t &dataLatch, &attrLatch;
        uint16_t dataAddr, attrAddr;

        uint32_t rgba;
        bool hSync, vSync;
        bool blank;
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
        uint8_t ioPortIn;
        uint8_t ioPortOut;

        // Keyboard half rows
        uint8_t keys[8];

        // ULA interrupt
        uint16_t c;
        uint32_t intCounter;
};
// vim: et:sw=4:ts=4
