#include "ULA.h"

#include <cassert>
#include <cmath>

using namespace std;

uint_fast32_t ULA::voltages[4][4] =
{
    {391, 728, 3653, 3790}, // ULA 5C (Issue 2)
    {342, 652, 3591, 3753}, // ULA 6C (Issue 3)
    {342, 652, 3591, 3753}, // ULA 7C (128K)
    {342, 652, 3591, 3753}  // GA40085 (+2A/+3)
};

bool ULA::delayTable[16] = 
{
    false, false, false, true, true, true, true, true,
    true, true, true, true, true, true, true, false
};

bool ULA::idleTable[16] =
{
    true, true, true, true, true, true, true, true,
    false, false, false, false, false, false, false, false
};

bool ULA::memTable[16] =
{
    true, true, true, true, true, true, true, true,
    false, true, false, true, false, true, false, true
};

uint32_t ULA::colourTable[0x100];
uint8_t ULA::averageTable[0x100][0x100];

ULA::ULA() :
    vSync(false), blanking(false), retrace(false),
    keys{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    c(0xFFFF)
{
    for (uint_fast32_t i = 0; i < 0x100; ++i)
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

    for (size_t i = 0x00; i < 0x100; ++i)
    {
        for (size_t j = 0x00; j < 0x100; ++j)
            averageTable[i][j] =
                static_cast<uint8_t>(sqrt(((i * i) + (j * j)) / 2));
    }
}

uint32_t ULA::average(uint32_t a, uint32_t b)
{
    uint32_t res;
    uint8_t *pRes = reinterpret_cast<uint8_t*>(&res);
    uint8_t *pa = reinterpret_cast<uint8_t*>(&a);
    uint8_t *pb = reinterpret_cast<uint8_t*>(&b);
#if SPECIDE_BYTE_ORDER == 1
    pRes[0] = 0xFF;
    pRes[1] = averageTable[pa[1]][pb[1]];
    pRes[2] = averageTable[pa[2]][pb[2]];
    pRes[3] = averageTable[pa[3]][pb[3]];
#else
    pRes[0] = averageTable[pa[0]][pb[0]];
    pRes[1] = averageTable[pa[1]][pb[1]];
    pRes[2] = averageTable[pa[2]][pb[2]];
    pRes[3] = 0xFF;
#endif
    return res;
}

void ULA::generateVideoControlSignals()
{
    if (pixel == videoStart)
    {
        video = (scan < vBorderStart);
    }
    else if (pixel == hBorderStart)
    {
        border = true;
    }
    else if (pixel == videoEnd)
    {
        video = false;
    }
    else if (pixel == hBlankStart)
    {
        blanking = true;
        ++scan;
        retrace = (scan >= vSyncStart) && (scan < vSyncEnd);

        if (scan == vBlankEnd)
        {
            flash += 0x04;
        }
        else if (scan == vSyncEnd)
        {
            vSync = true;
            frame = 1 - frame;
            yPos = 0;
        }
        else if (scan == maxScan)
        {
            scan = 0;
        }
    }
    else if (pixel == hSyncEnd)
    {
        xPos = 0;
        if (retrace == false)
            yPos += yInc;
    }
    else if (pixel == hBlankEnd)
    {
        blanking = (scan >= vBlankStart) && (scan <= vBlankEnd);
    }
    else if (pixel == maxPixel)
    {
        pixel = 0;
        border = (scan >= vBorderStart);
        ulaReset = false;
        dataAddr = ((scan & 0x38) << 2)          // 00076210 54376540
            | ((scan & 0x07) << 8)
            | ((scan & 0xC0) << 5);

        attrAddr = ((scan & 0xF8) << 2)          // 00000076 54376540
            | 0x1800;
    }
}

void ULA::generateInterrupt()
{

    if (interrupt)
        z80_c &= ~SIGNAL_INT_;
    else
        z80_c |= SIGNAL_INT_;

    if (pixel == interruptStart)
        interrupt = true;
    else if (pixel == interruptEnd)
        interrupt = false;
}

void ULA::generateVideoDataUla()
{
    if (!border)
    {
        // Check for contended memory or I/O accesses.
        idle = idleTable[pixel & 0x0F];
        mem = memTable[pixel & 0x0F];

        // Memory Contention
        // We detect memory contended states whenever the address is in the
        // contention range (0x4000-0x7FFF).
        // We only delay T1H until the ULA has finished reading. The rest of
        // states are not contended. We do this by checking MREQ is low.
        // We contend T-States, which means we only consider high clock phase.
        bool memContention = contendedBank && z80Clk;
        bool memContentionOff = ((z80_c & SIGNAL_MREQ_) == 0x0000);

        // I/O Contention
        // We detect I/O contended states whenever the address is even (A0 = 0)
        // and IORQ is low.
        // We only delay T2H. We do this by checking a delayed version of IORQ.
        bool ioUlaPort = ((z80_a & 0x0001) == 0x0000);
        bool iorqLow = ((z80_c & SIGNAL_IORQ_) == 0x0000);          // T2 TW T3
        bool iorqLow_d = ((z80_c_2 & SIGNAL_IORQ_) == 0x0000);      // TW T3 T1
        bool ioContention = ioUlaPort && iorqLow && z80Clk;         // T2 TW T3
        bool ioContentionOff = ioUlaPort && iorqLow_d;              // TW T3 NN

        // We use the same contention manager, and we consider contention when
        // there is any contention, and when no contention is not disabled.
        contention = (memContention || ioContention)        // Contention On?
            && !(memContentionOff || ioContentionOff);      // Contention Off?

        // Resolve contention and generate CPU clock.
        cpuClock = !(contention && delayTable[pixel & 0x0F]);

        // Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x08:
            case 0x0C:
                a = dataAddr++;
                break;
            case 0x0A:
            case 0x0E:
                a = attrAddr++;
                break;
            case 0x09:
            case 0x0D:
                dataReg = d;
                break;
            case 0x0B:
            case 0x0F:
                attrReg = d;
                break;
            default:
                break;
        }
    }
    else
    {
        idle = true;
    }
}

void ULA::generateVideoDataGa()
{
    if (!border)
    {
        // Check for contended memory or I/O accesses.
        idle = idleTable[pixel & 0x0F];
        mem = memTable[pixel & 0x0F];

        // Memory Contention
        // +2A/+3 only apply contention if MREQ is low.
        contention = contendedBank && ((z80_c & SIGNAL_MREQ_) == 0x0000);

        if (contention && delayTable[pixel & 0x0F])
            z80_c &= ~SIGNAL_WAIT_;
        else
            z80_c |= SIGNAL_WAIT_;

        // Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x08:
            case 0x0c:
                a = dataAddr++;
                break;
            case 0x0a:
            case 0x0e:
                a = attrAddr++;
                break;
            case 0x09:
            case 0x0d:
                dataReg = d;
                break;
            case 0x0b:
            case 0x0f:
                attrReg = d;
                break;
            default:
                break;
        }
    }
    else
    {
        if (delayTable[pixel & 0x0F] == false)
            z80_c |= SIGNAL_WAIT_;
    }
}

void ULA::paint()
{
    if ((pixel & 0x07) == 0x03)
    {
        data = video ? dataReg : 0xFF;
        attr = video ? attrReg : borderAttr;
        colour[0] = colourTable[(0x00 ^ (attr & flash & 0x80)) | (attr & 0x7F)];
        colour[1] = colourTable[(0x80 ^ (attr & flash & 0x80)) | (attr & 0x7F)];

        if (!blanking)
        {
            xPos += 8;

            uint32_t *ptr, *ptr1, *ptr2;
            switch (scanlines)
            {
                case 1:     // Scanlines
                    assert((((yPos + frame) * xSize) + xPos) < pixelsX2.size());
                    ptr1 = &pixelsX2[((yPos + frame) * xSize) + xPos];
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    --ptr1; *ptr1 = colour[data & 0x01]; data >>= 1;
                    break;

                case 2:     // Averaged scanlines
                    assert(((yPos * xSize) + xPos) < pixelsX1.size());
                    assert((((2 * yPos + frame) * xSize) + xPos) < pixelsX2.size());
                    assert((((2 * yPos + (1 - frame)) * xSize) + xPos) < pixelsX2.size());
                    ptr = &pixelsX1[(yPos * xSize) + xPos];
                    ptr1 = &pixelsX2[((2 * yPos + frame) * xSize) + xPos];
                    ptr2 = &pixelsX2[((2 * yPos + (1 - frame)) * xSize) + xPos];
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    --ptr; --ptr1; --ptr2; *ptr1 = colour[data & 0x01]; data >>= 1;
                    *ptr = average(*ptr2, *ptr1);
                    break;

                default:    // No scanlines
                    assert(((yPos * xSize) + xPos) < pixelsX1.size());
                    ptr = &pixelsX1[(yPos * xSize) + xPos];
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    --ptr; *ptr = colour[data & 0x01]; data >>= 1;
                    break;
            }
        }
    }
}

void ULA::tapeEarMic()
{
    // First attempt at MIC/EAR feedback loop.
    // Let's keep this here for the moment.
    if (ulaVersion < 2)
    {
        uint_fast32_t v = voltage[(ioPortOut & 0x18) >> 3];

        if (v > 3000)
        {
            ear = v;
            if (chargeDelay < 86400)
                ++chargeDelay;
            capacitor = 368 + (chargeDelay >> 4);
        }
        else
        {
            chargeDelay = 0;
            if (capacitor == 0)
                ear = v;
            else
                --capacitor;
        }
    }

    // Tape input forces values on EAR pin.
    if ((tapeIn & 0xC0) == 0x80) ear = 342;
    if ((tapeIn & 0xC0) == 0xC0) ear = 3790;
}

void ULA::ioPort()
{
    // We read keyboard if we're reading the ULA port, during TW.
    if (((z80_c & SIGNAL_IORQ_) == 0x0000) && ((z80_a & 0x0001) == 0x0000))
    {
        if ((z80_c & SIGNAL_RD_) == 0x0000)
        {
            if (++ioWait == 5)
            {
                ioWait = 0;
                ioPortIn = inMask;
                ioPortIn |= (ear < 700) ? 0x00 : 0x40;
                if ((z80_a & 0x8000) == 0x0000) ioPortIn &= keys[0];
                if ((z80_a & 0x4000) == 0x0000) ioPortIn &= keys[1];
                if ((z80_a & 0x2000) == 0x0000) ioPortIn &= keys[2];
                if ((z80_a & 0x1000) == 0x0000) ioPortIn &= keys[3];
                if ((z80_a & 0x0800) == 0x0000) ioPortIn &= keys[4];
                if ((z80_a & 0x0400) == 0x0000) ioPortIn &= keys[5];
                if ((z80_a & 0x0200) == 0x0000) ioPortIn &= keys[6];
                if ((z80_a & 0x0100) == 0x0000) ioPortIn &= keys[7];
                io = ioPortIn;
            }
        }
        else if ((z80_c & SIGNAL_WR_) == 0x0000)
        {
            ++ioWait;
            if (ioWait == 1)
            {
                ioPortOut = io;
                borderAttr = ioPortOut & 0x07;
            }
            else if (ioWait == 5)
            {
                ioWait = 0;
            }
        }
    }
}

void ULA::clock()
{
    generateVideoControlSignals();

    if (scan == vSyncStart)
        generateInterrupt();

    if (ulaVersion == 3)
        generateVideoDataGa();
    else
        generateVideoDataUla();
    tapeEarMic();

    // Contention affects to Z80 phase change and to when the I/O operation
    // actually happens.
    if (cpuClock == true && (z80_c & SIGNAL_WAIT_) == SIGNAL_WAIT_)
    {
        ioPort();
        z80_c_2 = z80_c_1;
        z80_c_1 = z80_c;
        z80Clk = !z80Clk;
    }

    paint();

    ++pixel;

    if (ulaReset)
        start();
}

void ULA::start()
{
    pixel = 0;
    scan = 0;
    ulaReset = false;
    z80Clk = false;
    video = true;
    border = false;

    z80_c_2 = z80_c_1 = 0xFFFF;

}

void ULA::reset()
{
    ulaReset = true;
}

void ULA::setUlaVersion(uint_fast8_t version)
{
    ulaVersion = version;

    videoStart = 0x008;
    videoEnd = 0x108;
    hBorderStart = 0x100;
    hBlankStart = 0x140;
    hBlankEnd = 0x19F;

    vBorderStart = 0x0C0;
    vBlankStart = 0x0F8;
    vBlankEnd = 0x0FF;
    vSyncStart = 0x0F8;
    vSyncEnd = 0x0FC;

    switch (ulaVersion)
    {
        case 0: // 48K, Issue 2
            hSyncEnd = 0x170;
            maxPixel = 0x1C0;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
        case 1: // 48K, Issue 3
            hSyncEnd = 0x178;
            maxPixel = 0x1C0;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
        case 2: // 128K, +2
            hSyncEnd = 0x178;
            maxPixel = 0x1C8;
            interruptStart = 0x004;
            interruptEnd = 0x044;
            maxScan = 0x137;
            break;
        case 3: // +2A, +3
            hSyncEnd = 0x178;
            maxPixel = 0x1C8;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x137;
            cpuClock = true;
            break;
        // case 4: // Pentagon
            // hSyncEnd = 0x158;
            // hBlankStart = 0x138;
            // hBlankEnd = 0x177;
            // maxPixel = 0x1C0;
            // vBlankStart = 0x0F0;
            // vSyncStart = 0x0F0;
            // maxScan = 0x140;
            // interruptStart = 0x000;
            // interruptEnd = 0x03F;
            // break;

        default:
            hSyncEnd = 0x178;
            maxPixel = 0x1C0;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
    }

    bool delayUla[16] = {
        false, false, false, true, true, true, true, true,
        true, true, true, true, true, true, true, false
    };
    bool idleUla[16] = {
        true, true, true, true, true, true, true, true,
        false, false, false, false, false, false, false, false
    };
    bool memUla[16] = {
        true, true, true, true, true, true, true, true,
        false, true, false, true, false, true, false, true
    };

    bool delayGa[16] = {
        true, true, false, false, true, true, true, true,
        true, true, true, true, true, true, true, true
    };
    bool idleGa[16] = {
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true
    };
    bool memGa[16] = {
        true, true, true, true, true, true, true, true,
        false, true, false, true, false, true, false, true
    };


    for (uint_fast8_t ii = 0; ii < 16; ++ii)
    {
        if (ulaVersion == 3)
        {
            delayTable[ii] = delayGa[ii];
            idleTable[ii] = idleGa[ii];
            memTable[ii] = memGa[ii];
        }
        else
        {
            delayTable[ii] = delayUla[ii];
            idleTable[ii] = idleUla[ii];
            memTable[ii] = memUla[ii];
        }
    }

    for (uint_fast32_t ii = 0; ii < 4; ++ii)
        voltage[ii] = voltages[ulaVersion][ii];
}
// vim: et:sw=4:ts=4:
