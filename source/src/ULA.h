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

        bool hiz;
        bool tState;

        size_t scan, maxScan;
        size_t pixel, maxPixel;

        uint16_t dataLatch, attrLatch;
        uint16_t dataAddr, attrAddr;

        uint_fast32_t r, g, b;
        bool hSync, vSync;
        bool blank;
        bool border;

};
// vim: et:sw=4:ts=4
