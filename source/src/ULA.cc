#include "ULA.h"

ULA::ULA() :
    as_(true), rd_(true),
    hiz(true),
    tState(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
    borderAttr(0),
    flash(0),
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
    for (size_t i = 0; i < 0x100; ++i)
    {
        // Generate colour table.
        // dhpppiii
        uint8_t r = (i & 0x80) ? ((i & 0x02) >> 1) : ((i & 0x10) >> 4);
        uint8_t g = (i & 0x80) ? ((i & 0x04) >> 2) : ((i & 0x20) >> 5);
        uint8_t b = (i & 0x80) ? ((i & 0x01) >> 0) : ((i & 0x08) >> 3);

#if SPECIDE_BYTE_ORDER == 1
        uint32_t colour = r << 24 | g << 16 | b << 8;
#else
        uint32_t colour = b << 16 | g << 8 | r;
#endif
        colour *= (i & 0x40) ? 0xFF : 0xC0;
#if SPECIDE_BYTE_ORDER == 1
        colour |= 0xFF;
#else
        colour |= (0xFF << 24);
#endif
        colourTable[i] = colour;
    }
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
        switch (pixel & 0x0F)
        {
            case 0x07:
                // Generate addresses (which must be pair).
                dataAddr = ((pixel & 0xF0) >> 3)    // 000SSSSS SSSPPPP0
                    | ((scan & 0x38) << 2)          // 00076210 54376540
                    | ((scan & 0x07) << 8)
                    | ((scan & 0xC0) << 5);

                attrAddr = ((pixel & 0xF0) >> 3)    // 000110SS SSSPPPP0
                    | ((scan & 0xF8) << 2)          // 00000076 54376540
                    | 0x1800;
                break;
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
    uint8_t colourIndex = ((data & 0x80) ^ (attr & flash)) | (attr & 0x7F);
    rgba = colourTable[colourIndex];

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

    // 5. Update counters
    pixel = (pixel + 1) % maxPixel;
    if (pixel == hSyncStart)
    {
        flash += (scan == vBlankEnd) ? 0x04 : 0x00;
        scan = (scan + 1) % maxScan;
    }
}
    
