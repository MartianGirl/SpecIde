#include "ULA.h"

int_fast32_t ULA::voltages[2][4] = {
    {391, 728, 3653, 3790}, // ULA 5C (Issue 2)
    {342, 652, 3591, 3753}  // ULA 6C (Issue 3)
};

size_t ULA::hSyncStart[2] = {0x150, 0x158};
size_t ULA::hSyncEnd[2] = {0x16F, 0x177};

ULA::ULA() :
    ulaVersion(1),
    maxPixel(448), maxScan(312),
    hiz(true),
    z80_a(0xFFFF), z80_c(0xFFFF),
    cpuClock(false), ulaReset(true),
    display(true), idle(false), 
    hSyncEdge(false), vSyncEdge(false), blanking(false), retrace(false),
    ioPortIn(0xFF), ioPortOut(0x00), inMask(0xBF), tapeIn(0),
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
}

void ULA::clock()
{
    static size_t pixel = 0;
    static size_t scan = 0;
    static uint_fast8_t flash = 0;
    static bool z80Clk = false;

    static uint_fast16_t z80_c_1 = 0xFFFF;
    static uint_fast16_t z80_c_2 = 0xFFFF;
    static uint_fast16_t z80_c_3 = 0xFFFF;
    static uint_fast16_t z80_c_4 = 0xFFFF;

    static uint_fast16_t dataAddr, attrAddr;

    static uint_fast8_t data;
    static uint_fast8_t attr;
    static uint_fast8_t dataLatch;
    static uint_fast8_t attrLatch;

    static size_t ear = 0;
    static size_t chargeDelay = 0;
    static size_t capacitor = 0;

    // Here we:
    // 1. Generate video control signals.
    if (pixel == hBorderStart)
    {
        display = false;
    }
    else if (pixel == hBlankStart)
    {
        blanking = true;
        ++scan;
        vSyncEdge = (scan == (vSyncEnd + 1));
        retrace = (scan >= vSyncStart) && (scan <= vSyncEnd);

        if (scan == vBlankEnd)
            flash += 0x04;
        else if (scan == maxScan)
            scan = 0;
    }
    else if (pixel == (hSyncEnd[ulaVersion] + 1))
    {
        hSyncEdge = true;
    }
    else if (pixel == hBlankEnd)
    {
        blanking = (scan >= vBlankStart) && (scan <= vBlankEnd);
    }
    else if (pixel == maxPixel)
    {
        pixel = 0;
        display = (scan < vBorderStart);
        ulaReset = false;
    }
    else
    {
        hSyncEdge = false;
        vSyncEdge = false;
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
                data = dataLatch; attr = attrLatch;
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
                idle = true; delay = false; hiz = true; break;
            case 0x0F:
                data = dataLatch; attr = attrLatch;
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

        if ((pixel & 0x07) == 0x07)
        {
            data = 0xFF;
            attr = borderAttr;
        }
    }

    // 3. ULA port & Interrupt.
    c = z80_c;
    if ((scan == vSyncStart) && (pixel < 64))
            // && ((z80_c & (SIGNAL_M1_ | SIGNAL_IORQ_)) != 0x0000))
        c &= ~SIGNAL_INT_;
    else
        c |= SIGNAL_INT_;

    // First attempt at MIC/EAR feedback loop.
    // Let's keep this here for the moment.
    size_t voltage = voltages[ulaVersion][(ioPortOut & 0x18) >> 3];

    if (voltage > 3000)
    {
        ear = voltage;
        if (chargeDelay < 86400)
            ++chargeDelay;
        capacitor = 368 + (chargeDelay >> 4);
    }
    else
    {
        chargeDelay = 0;
        if (capacitor == 0)
            ear = voltage;
        else
            --capacitor;
    }

    // Tape input forces values on EAR pin.
    if ((tapeIn & 0xC0) == 0x80) ear = 342;
    if ((tapeIn & 0xC0) == 0xC0) ear = 3790;

    // We read keyboard if we're reading the ULA port, during TW.
    if (cpuClock)   // Port read access is contended.
    {
        if (((z80_a & 0x0001) == 0x0000) && ((z80_c & SIGNAL_IORQ_) == 0x0000))
        {
            if (((z80_c | z80_c_4) & SIGNAL_RD_) == 0x0000)
            {
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

    // 4. Generate video signal. Generate colours.
    rgba = colourTable[((data & 0x80) ^ (attr & flash)) | (attr & 0x7F)];

    // 4.b. Update data and attribute registers.
    data <<= 1;

    // 5. Update counters
    ++pixel;

    if (ulaReset)
    {
        pixel = 0;
        scan = 0;
        ulaReset = false;
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
