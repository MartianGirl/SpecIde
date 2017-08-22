#include "ULA.h"

/*
size_t ULA::pixelStart = 0;
size_t ULA::pixelEnd = 0xFF;
size_t ULA::hBorderStart = 0x100;
size_t ULA::hBorderEnd = 0x1BF;
size_t ULA::hBlankStart = 0x140;
size_t ULA::hBlankEnd = 0x19F;
size_t ULA::hSyncStart = 0x150;
size_t ULA::hSyncEnd = 0x16F;
//hSyncStart(0x158), hSyncEnd(0x177),     // ULA 6C (Issue 3)

size_t ULA::scanStart = 0;
size_t ULA::scanEnd = 0x0BF;
size_t ULA::vBorderStart = 0x0C0;
size_t ULA::vBorderEnd = 0x137;
size_t ULA::vBlankStart = 0x0F8;
size_t ULA::vBlankEnd = 0x0FF;
size_t ULA::vSyncStart = 0x0F8;
size_t ULA::vSyncEnd = 0x0FB;

size_t ULA::maxScan = 312;
size_t ULA::maxPixel = 448;
*/

int_fast32_t ULA::tensions[2][4] = {
    {391, 728, 3653, 3790}, // ULA 5C (Issue 2)
    {342, 652, 3591, 3753}  // ULA 6C (Issue 3)
};
int_fast32_t ULA::constants[] = {0};

size_t ULA::hSyncStart[2] = {0x150, 0x158};
size_t ULA::hSyncEnd[2] = {0x16F, 0x177};

ULA::ULA() :
    ulaVersion(0),
    maxPixel(448), maxScan(312),
    hiz(true),
    z80_a(0xFFFF), z80_c(0xFFFF),
    cpuClock(false), ulaReset(true),
    hBlank(false), vBlank(false), display(true), idle(false), 
    ioPortIn(0xFF), ioPortOut(0x00), tapeIn(0),
    c00(996), c01(996), c10(392), c11(392),
    keys{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    c(0xFFFF)
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

    int_fast32_t v00, v01, v10, v11;
    v00 = v01 = v10 = v11 = 1000;
    for (size_t i = 0; i < 1024; ++i)
    {
        constants[i + 0] = v00 * 1024 / 1000;
        constants[i + 1024] = v01 * 1024 / 1000;
        constants[i + 2048] = v10 * 1024 / 1000;
        constants[i + 3072] = v11 * 1024 / 1000;
        v00 *= c00; v00 /= 1000;
        v01 *= c01; v01 /= 1000;
        v10 *= c10; v10 /= 1000;
        v11 *= c11; v11 /= 1000;
    }
}

void ULA::clock()
{
    static size_t cIndex = 0;
    static size_t pixel = 0;
    static size_t scan = 0;
    static uint_fast8_t flash = 0;
    static bool z80Clk = false;

    static uint_fast16_t z80_c_1 = 0xFFFF;
    static uint_fast16_t z80_c_2 = 0xFFFF;
    static uint_fast16_t z80_c_3 = 0xFFFF;
    static uint_fast16_t z80_c_4 = 0xFFFF;

    static uint_fast16_t dataAddr, attrAddr;
    static uint_fast32_t dataReg, attrReg;
#if SPECIDE_BYTE_ORDER == 1
    static uint8_t &data = (*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint_fast32_t) - 3));
    static uint8_t &attr = (*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint_fast32_t) - 3));
    static uint8_t &dataLatch = (*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint_fast32_t) - 1));
    static uint8_t &attrLatch = (*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint_fast32_t) - 1));
#else
    static uint8_t &data = (*(reinterpret_cast<uint8_t*>(&dataReg) + 2));
    static uint8_t &attr = (*(reinterpret_cast<uint8_t*>(&attrReg) + 2));
    static uint8_t &dataLatch = (*(reinterpret_cast<uint8_t*>(&dataReg) + 0));
    static uint8_t &attrLatch = (*(reinterpret_cast<uint8_t*>(&attrReg) + 0));
#endif

    static uint_fast32_t capacitor = 0;
    static size_t outputCurr = 0;
    static size_t outputLast = 0;
    static int_fast32_t vStart = 0;
    static int_fast32_t vEnd = 0;
    static int_fast32_t vDiff = 0;

    // Here we:
    // 1. Generate video control signals.
    if (pixel == hBorderStart)
    {
        display = false;
    }
    else if (pixel == hBlankStart)
    {
        hBlank = true;
        ++scan;
        vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);
        vBlank = ((scan >= vBlankStart) && (scan <= vBlankEnd));

        if (scan == vBlankEnd)
            flash += 0x04;
        else if (scan == maxScan)
            scan = 0;
    }
    else if (pixel == hSyncStart[ulaVersion])
    {
        hSync = true;
    }
    else if (pixel == hSyncEnd[ulaVersion])
    {
        hSync = false;
    }
    else if (pixel == hBlankEnd)
    {
        hBlank = false;
    }
    else if (pixel == maxPixel)
    {
        pixel = 0;
        display = (scan < vBorderStart);
        ulaReset = false;
    }

    // 2. Generate video data.
    if (display)
    {
        bool delay;
        // 2.a. Check for contended memory or I/O accesses.

        // Memory Contention
        // We detect memory contended states whenever the address is in the
        // contention range (0x4000-0x7FFF).
        // We only delay T1H until the ULA has finished reading. The rest of
        // states are not contended. We do this by checking MREQ is low.
        // We contend T-States, which means we only consider high clock phase.
        bool memContention = ((z80_a & 0xC000) == 0x4000) && z80Clk;
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
        bool contention = (memContention || ioContention)   // Contention On?
            && !(memContentionOff || ioContentionOff);      // Contention Off?

        // 2.b. Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
                idle = true; delay = true; hiz = true; break;
            case 0x04:
                // Generate addresses (which must be pair).
                dataAddr = ((pixel & 0xF0) >> 3)    // 000SSSSS SSSPPPP0
                    | ((scan & 0x38) << 2)          // 00076210 54376540
                    | ((scan & 0x07) << 8)
                    | ((scan & 0xC0) << 5);

                attrAddr = ((pixel & 0xF0) >> 3)    // 000110SS SSSPPPP0
                    | ((scan & 0xF8) << 2)          // 00000076 54376540
                    | 0x1800;
                a = dataAddr;
                idle = true; delay = true; hiz = false; break;
            case 0x05:
                dataLatch = d;
                idle = true; delay = true; hiz = false; break;
            case 0x06:
                a = attrAddr;
                idle = false; delay = true; hiz = false; break;
            case 0x07:
                attrLatch = d;
                idle = false; delay = true; hiz = false; break;
            case 0x08:
                a = dataAddr + 1;
                idle = false; delay = true; hiz = false; break;
            case 0x09:
                dataLatch = d;
                idle = false; delay = true; hiz = false; break;
            case 0x0A:
                a = attrAddr + 1;
                idle = false; delay = true; hiz = false; break;
            case 0x0B:
                attrLatch = d;
                idle = false; delay = true; hiz = false; break;
            case 0x0C:
            case 0x0D:
                idle = false; delay = false; hiz = true; break;
            case 0x0E:
            case 0x0F:
                idle = true; delay = false; hiz = true; break;
            default:
                a = 0xFFFF;
                idle = true; delay = false; hiz = true; break;
        }

        // 2.c. Resolve contention and generate CPU clock.
        cpuClock = !(contention && delay);
    }
    else
    {
        hiz = true;
        cpuClock = true;
        idle = true;
    }

    // 3. ULA port & Interrupt.
    c = z80_c;
    if ((scan == vSyncStart) && (pixel < 64)
            && ((z80_c & (SIGNAL_M1_ | SIGNAL_IORQ_)) != 0x0000))
        c &= ~SIGNAL_INT_;
    else
        c |= SIGNAL_INT_;

    // First attempt at MIC/EAR feedback loop.
    // Let's keep this here for the moment.
    outputLast = outputCurr;
    outputCurr = (ioPortOut & 0x18) >> 3;

    if (outputLast != outputCurr)
    {
        cIndex = 0;
        vStart = capacitor;
        vEnd = tensions[ulaVersion][outputCurr];
        vDiff = vStart - vEnd;
    }
    else
    {
        capacitor = vEnd + 
            ((vDiff * constants[cIndex + 1024 * outputCurr]) >> 10);
        ++cIndex;
        if (cIndex > 1023) cIndex = 1023;
    }

    if (capacitor > 5000) capacitor = 5000;
    if ((tapeIn & 0xC0) == 0x80) capacitor = 650;
    if ((tapeIn & 0xC0) == 0xC0) capacitor = 5000;
    ioPortIn &= 0xBF;
    ioPortIn |= (capacitor < 700) ? 0x00 : 0x40;

    // We read keyboard if we're reading the ULA port, during TW.
    if (cpuClock)   // Port read access is contended.
    {
        if (((z80_a & 0x0001) == 0x0000) && ((z80_c & SIGNAL_IORQ_) == 0x0000))
        {
            if (((z80_c | z80_c_4) & SIGNAL_RD_) == 0x0000)
            {
                ioPortIn |= 0x1F;
                if ((z80_a & 0x8000) == 0x0000) ioPortIn &= keys[0];
                if ((z80_a & 0x4000) == 0x0000) ioPortIn &= keys[1];
                if ((z80_a & 0x2000) == 0x0000) ioPortIn &= keys[2];
                if ((z80_a & 0x1000) == 0x0000) ioPortIn &= keys[3];
                if ((z80_a & 0x0800) == 0x0000) ioPortIn &= keys[4];
                if ((z80_a & 0x0400) == 0x0000) ioPortIn &= keys[5];
                if ((z80_a & 0x0200) == 0x0000) ioPortIn &= keys[6];
                if ((z80_a & 0x0100) == 0x0000) ioPortIn &= keys[7];
                d = ioPortIn;
            }

            if (((z80_c | z80_c_4) & SIGNAL_WR_) == 0x0000)
            {
                ioPortOut = d;
                borderAttr = ioPortOut & 0x07;
            }
        }

        z80_c_4 = z80_c_3;
        z80_c_3 = z80_c_2;
        z80_c_2 = z80_c_1;
        z80_c_1 = z80_c;

        z80Clk = !z80Clk;
    }

    // 4. Generate video signal.
    // 4.a. Generate colours.
    rgba = colourTable[((data & 0x80) ^ (attr & flash)) | (attr & 0x7F)];

    // 4.b. Update data and attribute registers.
    data <<= 1;

    // We start outputting data after just a data/attr pair has been fetched.
    if ((pixel & 0x07) == 0x00)
    {
        // This actually causes the following, due to the placement of the
        // aliases:
        // data = dataOut; attr = attrOut;
        // dataOut = dataLatch; attrOut = attrLatch;
        // attrLatch = ioPortOut; dataLatch = 0xFF;
        dataReg <<= 8;
        attrReg <<= 8;
        dataLatch = 0xFF;
        attrLatch = borderAttr;
    }

    // 5. Update counters
    ++pixel;

    if (ulaReset)
    {
        pixel = 0;
        scan = 0;
        ulaReset = false;
        cIndex = 0;
        z80Clk = false;
        display = true;

        z80_c_4 = z80_c_3 = z80_c_2 = z80_c_1 = 0xFFFF;
    }
}

void ULA::reset()
{
    ulaReset = true;
}
// vim: et:sw=4:ts=4:
