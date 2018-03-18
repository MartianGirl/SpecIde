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

        void generateVideoControlSignals();
        void generateInterrupt();
        void generateVideoData();
        void tapeEarMic();
        void ioPort();
        void start();

        void setUlaVersion(uint_fast8_t version);

        uint_fast16_t hBorderStart;
        uint_fast16_t hBlankStart;
        uint_fast16_t hBlankEnd;
        uint_fast16_t hSyncEnd;
        uint_fast16_t maxPixel;
        uint_fast16_t interruptStart;
        uint_fast16_t interruptEnd;

        uint_fast16_t vBorderStart;
        uint_fast16_t vBlankStart;
        uint_fast16_t vBlankEnd;
        uint_fast16_t vSyncStart;
        uint_fast16_t vSyncEnd;
        uint_fast16_t maxScan;

        uint_fast32_t voltage[4];

        static uint_fast32_t voltages[4][4];
        static bool delayTable[16];
        static bool idleTable[16];
        static bool memTable[16];

        static uint32_t colourTable[0x100];
        uint32_t colour0, colour1;

        // These values depend on the model
        uint_fast8_t ulaVersion = 1;

        // ULA internals
        uint_fast16_t pixel = 0;
        uint_fast16_t scan = 0;
        uint_fast8_t flash = 0;
        bool z80Clk = false;

        uint_fast8_t rdWait = 0;
        uint_fast16_t z80_c_1 = 0xFFFF;
        uint_fast16_t z80_c_2 = 0xFFFF;

        uint_fast16_t dataAddr, attrAddr;

        uint_fast8_t data;
        uint_fast8_t attr;
        uint_fast8_t dataLatch;
        uint_fast8_t attrLatch;

        uint_fast32_t ear = 0;
        uint_fast32_t chargeDelay = 0;
        uint_fast32_t capacitor = 0;

        bool interruptRange = false;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        uint_fast8_t io;

        uint_fast16_t z80_a = 0xFFFF;
        uint_fast16_t z80_c = 0xFFFF;
        bool contendedBank = false;
        bool mreq_ = true;
        bool cpuClock = false;
        bool ulaReset = true;
        bool contention = false;

        // Video signals
        uint32_t rgba;
        bool display = true;
        bool idle = true;
        bool mem = true;

        // Useful video signals
        bool hSyncEdge;
        bool vSyncEdge;
        bool blanking;
        bool retrace;

        // Port 0xFE
        uint_fast8_t ioPortIn = 0xFF;
        uint_fast8_t ioPortOut = 0x00;
        uint_fast8_t inMask = 0xBF;
        uint_fast8_t borderAttr = 0x00;
        uint_fast8_t tapeIn = 0x00;

        // Keyboard half rows
        uint_fast8_t keys[8];

        // ULA interrupt
        uint_fast16_t c;
};
// vim: et:sw=4:ts=4
