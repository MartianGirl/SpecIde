#include "ULA.h"

ULA::ULA() :
    as_(true), rd_(true),
    hiz(true),
    tState(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
    borderAttr(0),
    flash(0x0000),
    data(0), attr(0), dataOut(0), attrOut(0), dataLatch(0), attrLatch(0),
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
    //    These are generic, but maybe some adjusting can be done for
    //    performance.
    hSync = (pixel >= hSyncStart) && (pixel <= hSyncEnd);
    vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);
    blank = ((pixel >= hBlankStart) && (pixel <= hBlankEnd))
        || ((scan >= vBlankStart) && (scan <= vBlankEnd));
    border = ((pixel >= hBorderStart) && (pixel <= hBorderEnd))
        || ((scan >= vBorderStart) && (scan <= vBorderEnd));

    // 2. Generate video data. Flash is missing so far.
    if (border == false)
    {
        // 2.a. Generate addresses (must be pair).
        dataAddr = ((pixel & 0xF0) >> 3)    // 000SSSSS SSSPPPP0
            | ((scan & 0x38) << 2)          // 00076210 54376540
            | ((scan & 0x07) << 8)
            | ((scan & 0xC0) << 5);

        attrAddr = ((pixel & 0xF0) >> 3)    // 000110SS SSSPPPP0
            | ((scan & 0xF8) << 2)          // 00000076 54376540
            | 0x1800;

        // 2.b. Memory reads.
        switch (pixel & 0x0F)
        {
            case 0x08:
                a = dataAddr; hiz = as_ = rd_ = false; break;
            case 0x09:
                dataLatch = d; break;
            case 0x0A:
                a = attrAddr; hiz = as_ = rd_ = false; break;
            case 0x0B:
                attrLatch = d; break;
            case 0x0C:
                a = dataAddr + 1; hiz = as_ = rd_ = false; break;
            case 0x0D:
                dataLatch = (dataLatch << 8) | d; break;
            case 0x0E:
                a = attrAddr + 1; hiz = as_ = rd_ = false; break;
            case 0x0F:
                attrLatch = (attrLatch << 8) | d; break;
            default:
                a = 0xFFFF; hiz = as_ = rd_ = true; break;
        }
    }

    // 3. Generate colours.
    r = ((data ^ (flash & attr)) & 0x80) ?
        ((attr & 0x02) >> 1) : ((attr & 0x10) >> 4);
    g = ((data ^ (flash & attr)) & 0x80) ? 
        ((attr & 0x04) >> 2) : ((attr & 0x20) >> 5);
    b = ((data ^ (flash & attr)) & 0x80) ?
        ((attr & 0x01) >> 0) : ((attr & 0x08) >> 3);

    r *= (attr & 0x40) ? 0xFF : 0xC0;
    g *= (attr & 0x40) ? 0xFF : 0xC0;
    b *= (attr & 0x40) ? 0xFF : 0xC0;

    // 4. Update data and attribute registers.
    data <<= 1;

    if ((pixel & 0x07) == 0x07) 
    {
        attr = attrOut;
        data = dataOut;

        attrOut = (attrLatch & 0xFF00) >> 8;
        dataOut = (dataLatch & 0xFF00) >> 8;
        attrLatch = (attrLatch << 8) | borderAttr;
        dataLatch = (dataLatch << 8);
    }

    // 5. Count pixels.
    pixel = (pixel + 1) % maxPixel;

    // 6. Count scans.
    if (pixel == hSyncStart)
    {
        flash += (scan == vBlankEnd) ? 0x04 : 0x00;
        scan = (scan + 1) % maxScan;
    }
}
    
