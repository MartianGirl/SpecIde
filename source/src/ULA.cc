#include "ULA.h"

ULA::ULA() :
    hiz(true),
    tState(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
    pixelStart(0), pixelEnd(255),
    hBorderStart(256), hBorderEnd(447),
    hBlankStart(320), hBlankEnd(415),
    hSyncStart(344), hSyncEnd(375),
    scanStart(0), scanEnd(191),
    vBorderStart(192), vBorderEnd(311),
    vBlankStart(248), vBlankEnd(255),
    vSyncStart(248), vSyncEnd(251)
{
}

void ULA::clock()
{
    // Here we:
    // 1. Generate video control signals.
    hSync = (pixel >= hSyncStart) && (pixel <= hSyncEnd);
    vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);
    blank = ((pixel >= hBlankStart) && (pixel <= hBlankEnd))
        || ((scan >= vBlankStart) && (scan <= vBlankEnd));
    border = ((pixel >= hBorderStart) && (pixel <= hBorderEnd))
        || ((scan >= vBorderStart) && (scan <= vBorderEnd));

    // 2. Generate video signal.
    // Let's do some dummy signal: Cyan display, blue border.
    r = 0x00;
    g = (border) ? 0xC0 : 0x00;
    b = 0xC0;

    // 3. Generate addresses.
    if (scan < 192 && pixel < 256)
    {
        dataAddr = (pixel >> 3)         // 000SSSSS SSSPPPPP
            | ((scan & 0x38) << 2)      // 00076210 54376543
            | ((scan & 0x07) << 8)
            | ((scan & 0xC0) << 5);

        attrAddr = (pixel >> 3)
            | ((scan & 0x38) << 2)
            | ((scan & 0xC0) << 3)
            | 0x1800;
    }
    // 4. Count pixels.
    pixel = (pixel + 1) % maxPixel;

    // 5. Count scans.
    if (pixel == hSyncStart)
        scan = (scan + 1) % maxScan;
}
    
