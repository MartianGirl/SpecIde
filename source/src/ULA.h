#pragma once

/** ULA
 *
 * Models a ZX Spectrum ULA.
 *
 */

#include <cstdint>

using namespace std;

class ULA
{
    public:
        ULA();

        void clock();

        uint16_t a;
        uint16_t c;
        uint8_t d;

        bool as_, rd_;

        bool hiz;
        bool tState;

        size_t scan, maxScan;
        size_t pixel, maxPixel;

        uint8_t borderAttr;
        uint8_t flash;
        uint32_t dataReg, attrReg;
        uint8_t &data, &attr;
        uint8_t &dataLatch, &attrLatch;
        uint16_t dataAddr, attrAddr;

        uint32_t rgba;
        bool hSync, vSync;
        bool blank;
        bool border;

        size_t pixelStart, pixelEnd;
        size_t hBorderStart, hBorderEnd;
        size_t hBlankStart, hBlankEnd;
        size_t hSyncStart, hSyncEnd;

        size_t scanStart, scanEnd;
        size_t vBorderStart, vBorderEnd;
        size_t vBlankStart, vBlankEnd;
        size_t vSyncStart, vSyncEnd;

        uint32_t colourTable[0x100];
};
// vim: et:sw=4:ts=4
