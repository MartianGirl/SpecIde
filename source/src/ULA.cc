#include "ULA.h"

ULA::ULA() :
    as_(true), rd_(true),
    hiz(true),
    tState(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
    borderAttr(0),
    flash(0),
    dataReg(0), attrReg(0),
#if SPECIDE_BYTE_ORDER == 1
    data(*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint32_t) - 3)),
    attr(*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint32_t) - 3)),
    dataLatch(*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint32_t) - 1)),
    attrLatch(*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint32_t) - 1)),
#else
    data(*(reinterpret_cast<uint8_t*>(&dataReg) + 2)),
    attr(*(reinterpret_cast<uint8_t*>(&attrReg) + 2)),
    dataLatch(*(reinterpret_cast<uint8_t*>(&dataReg) + 0)),
    attrLatch(*(reinterpret_cast<uint8_t*>(&attrReg) + 0)),
#endif
    blank(false), border(false),
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
#if SPECIDE_BYTE_ORDER == 1
        uint32_t colour = (i & 0x80) ?
            ((i & 0x02) << 23) | ((i & 0x04) << 14) | ((i & 0x01) << 8) :
            ((i & 0x10) << 20) | ((i & 0x20) << 11) | ((i & 0x08) << 5);
#else
        uint32_t colour = (i & 0x80) ?
            ((i & 0x02) >> 1) | ((i & 0x04) << 6) | ((i & 0x01) << 16) :
            ((i & 0x10) >> 4) | ((i & 0x20) << 3) | ((i & 0x08) << 13);
#endif
        colour *= (i & 0x40) ? 0xF0 : 0xC0;
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

    // 2. Generate video data.
    if (!border)
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
                dataLatch = d; break;
            case 0x0E:
                a = attrAddr + 1; hiz = as_ = rd_ = false; break;
            case 0x0F:
                attrLatch = d; break;
            default:
                a = 0xFFFF; hiz = as_ = rd_ = true; break;
        }
    }

    if (!blank)
    {
        // 3. Generate colours.
        rgba = colourTable[((data & 0x80) ^ (attr & flash)) | (attr & 0x7F)];

        // 4. Update data and attribute registers.
        data <<= 1;

        // We start outputting data after just a data/attr pair has been fetched.
        if ((pixel & 0x07) == 0x03)
        {
            // This actually causes the following, due to the placement of the
            // aliases:
            // data = dataOut; attr = attrOut;
            // dataOut = dataLatch; attrOut = attrLatch;
            // attrLatch = borderAttr; dataLatch = 0;
            dataReg <<= 8;
            attrReg <<= 8;
            attrLatch = borderAttr;
        }
    }

    // 5. Update counters
    pixel = (pixel + 1) % maxPixel;
    if (pixel == hSyncStart)
    {
        scan = (scan + 1) % maxScan;
        if (scan == vBlankEnd)
            flash += 0x04;
    }
}
    
