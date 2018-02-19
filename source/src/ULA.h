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

        // Constant-ish statics.
        static constexpr size_t pixelStart = 0x000;
        static constexpr size_t pixelEnd = 0x0FF;
        static constexpr size_t hBorderStart = 0x100;
        static constexpr size_t hBorderEnd = 0x1BF;
        static constexpr size_t hBlankStart = 0x140;
        static constexpr size_t hBlankEnd = 0x19F;
        static constexpr size_t hSyncStartULA5 = 0x150;
        static constexpr size_t hSyncStartULA6 = 0x158;
        static constexpr size_t hSyncEndULA5 = 0x170; // +1
        static constexpr size_t hSyncEndULA6 = 0x178; // +1
        static constexpr size_t maxPixel48K = 0x1C0;
        static constexpr size_t maxPixel128K = 0x1C8;
        static constexpr size_t interruptStart48K = 0x000;
        static constexpr size_t interruptEnd48K = 0x03F;
        static constexpr size_t interruptStart128K = 0x004;
        static constexpr size_t interruptEnd128K = 0x04B;

        static constexpr size_t scanStart = 0x000;
        static constexpr size_t scanEnd = 0x0BF;
        static constexpr size_t vBorderStart = 0x0C0;
        static constexpr size_t vBorderEnd = 0x137;
        static constexpr size_t vBlankStart = 0x0F8;
        static constexpr size_t vBlankEnd = 0x0FF;
        static constexpr size_t vSyncStart = 0x0F8;
        static constexpr size_t vSyncEnd = 0x0FC; // +1
        static constexpr size_t maxScan48K = 0x138;
        static constexpr size_t maxScan128K = 0x137;

        int_fast32_t voltage[4];

        static int_fast32_t voltages[3][4];
        static bool delayTable[16];
        static bool idleTable[16];
        static bool hizTable[16];

        static uint32_t colourTable[0x100];
        uint32_t colour0, colour1;

        // These values depend on the model
        size_t ulaVersion = 1;

        // ULA internals
        size_t pixel = 0;
        size_t scan = 0;
        uint_fast8_t flash = 0;
        bool z80Clk = false;

        size_t rdWait = 0;
        uint_fast16_t z80_c_1 = 0xFFFF;
        uint_fast16_t z80_c_2 = 0xFFFF;

        uint_fast16_t dataAddr, attrAddr;

        uint_fast8_t data;
        uint_fast8_t attr;
        uint_fast8_t dataLatch;
        uint_fast8_t attrLatch;

        size_t ear = 0;
        size_t chargeDelay = 0;
        size_t capacitor = 0;

        bool interruptRange = false;

        // Memory signals
        uint_fast16_t a;
        uint_fast8_t d;
        bool hiz = true;

        uint_fast16_t z80_a = 0xFFFF;
        uint_fast16_t z80_c = 0xFFFF;
        uint_fast16_t z80_mask = 0xFFFF;
        bool cpuClock = false;
        bool ulaReset = true;

        // Video signals
        uint32_t rgba;
        bool display = true;
        bool idle = true;

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
