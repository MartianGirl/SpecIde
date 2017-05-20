#include "ULA.h"

ULA::ULA() :
    hiz(true),
    tState(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448)
{
}

void ULA::clock()
{
    // Here we:
    // 1. Count pixels.
    pixel = (pixel + 1) % maxPixel;

    // 2. Count scans.
    if (pixel == 0)
        scan = (scan + 1) % maxScan;

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



}
    
