#include "ULA.h"

ULA::ULA() :
    hiz(true),
    z80_a(0xFFFF), z80_c(0xFFFF),
    cpuClock(false),
    maxScan(312),      // PAL values. Must make this configurable.
    maxPixel(448),
    hBlank(false), vBlank(false),
    pixelStart(0x000), pixelEnd(0x0FF),
    hBorderStart(0x100), hBorderEnd(0x1BF),
    hBlankStart(0x140), hBlankEnd(0x19F),
    hSyncStart(0x150), hSyncEnd(0x16F),     // ULA 5C (Issue 2)
    //hSyncStart(0x158), hSyncEnd(0x177),     // ULA 6C (Issue 3)
    scanStart(0x000), scanEnd(0x0BF),
    vBorderStart(0x0C0), vBorderEnd(0x137),
    vBlankStart(0x0F8), vBlankEnd(0x0FF),
    vSyncStart(0x0F8), vSyncEnd(0x0FB),
    ioPortIn(0xFF), ioPortOut(0x00), tapeIn(0),
    c00(996), c01(996), c10(392), c11(392),
    tensions{391, 728, 3653, 3790}, // ULA 5C (Issue 2)
    //tensions{342, 652, 3591, 3753}, // ULA 6C (Issue 3)
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

    static uint_fast16_t z80_c_d = 0xFFFF;
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

    uint_fast8_t d_i = d;
    uint_fast8_t d_o = d_i;

    // Here we:
    // 1. Generate video control signals.
    hSync = (pixel >= hSyncStart) && (pixel <= hSyncEnd);
    vSync = ((scan & 0x0FC) == 0x0F8);
    // vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);

    vBlank = ((scan & 0x0F8) == 0x0F8);
    hBlank = ((pixel >= hBlankStart) && (pixel <= hBlankEnd));

    bool display = (pixel < hBorderStart) && (scan < vBorderStart);

    // This is the "dead cockroach modification".
    //if ((z80_c & SIGNAL_IORQ_) == 0x0000)
    //{
        //z80_a &= 0xBFFF;
        //z80_a |= 0x4000;
    //}

    // 2. Generate video data.
    bool cpuWait;
    if (display)
    {
        bool delay;
        // 2.a. Check for contended memory or I/O accesses.
        // T1 of a memory cycle can be detected by a falling edge on MREQ.
        // T2 of an I/O cycle can be detected by a falling edge on IORQ.
        bool memAccess = ((z80_a & 0xC000) == 0x4000);
        bool mreqLow_d = ((z80_c_d & SIGNAL_MREQ_) == 0x0000);  // T2 T3
        bool memContention = memAccess && !mreqLow_d;           // T1 (T2 TW T3)
        //bool mreqEdge = ((~z80_c & z80_c_d & SIGNAL_MREQ_) == SIGNAL_MREQ_);
        //bool memContention = memAccess && mreqEdge; // Mem Access T1

        bool ioAccess = ((z80_a & 0x0001) == 0x0000);
        bool iorqLow = ((z80_c & SIGNAL_IORQ_) == 0x0000);      // T2 TW
        bool iorqLow_d = ((z80_c_d & SIGNAL_IORQ_) == 0x0000);  // TW T3
        bool ioContention = ioAccess && iorqLow;                // IO T2 TW
        bool contentionOff = ioAccess && iorqLow_d;             // IO TW T3
        bool contention = (memContention || ioContention) && !contentionOff;

        // T               1  2  W  3
        // ioContention1 : 0  1  0  0
        // ioContention2 : 0  0  0  0 => A != 4000 - 7FFF
        // ioContention2 : 1  1  0  0 => A0 = 0, A = 4000 - 7FFF
        // ioContention2 : 1  1  1  1 => A0 = 1, A = 4000 - 7FFF

        // 2.b. Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
                delay = false; hiz = true; break;
            case 0x04:
            case 0x05:
            case 0x06:
                delay = true; hiz = true; break;
            case 0x07:
                // Generate addresses (which must be pair).
                dataAddr = ((pixel & 0xF0) >> 3)    // 000SSSSS SSSPPPP0
                    | ((scan & 0x38) << 2)          // 00076210 54376540
                    | ((scan & 0x07) << 8)
                    | ((scan & 0xC0) << 5);

                attrAddr = ((pixel & 0xF0) >> 3)    // 000110SS SSSPPPP0
                    | ((scan & 0xF8) << 2)          // 00000076 54376540
                    | 0x1800;
                delay = true; hiz = true; break;
            case 0x08:
                a = dataAddr;
                delay = true; hiz = false; break;
            case 0x09:
                dataLatch = d_i;
                d_o = d_i;
                delay = true; hiz = false; break;
            case 0x0A:
                a = attrAddr;
                delay = true; hiz = false; break;
            case 0x0B:
                attrLatch = d_i;
                d_o = d_i;
                delay = true; hiz = false; break;
            case 0x0C:
                a = dataAddr + 1;
                delay = true; hiz = false; break;
            case 0x0D:
                dataLatch = d_i;
                d_o = d_i;
                delay = true; hiz = false; break;
            case 0x0E:
                a = attrAddr + 1;
                delay = true; hiz = false; break;
            case 0x0F:
                attrLatch = d_i;
                d_o = d_i;
                delay = true; hiz = false; break;
            default:
                a = 0xFFFF;
                delay = false; hiz = true; break;
        }

        // 2.c. Resolve contention and generate CPU clock.
        cpuWait = (contention && delay);
    }
    else
    {
        hiz = true;
        cpuWait = false;
    }
    
    cpuClock = !cpuWait && ((pixel & 0x0001) == 0x0001);

    // 3. ULA port & Interrupt.
    c = z80_c;
    if ((scan == 0xF8) && (pixel < 64)) 
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
        vEnd = tensions[outputCurr];
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
    if (cpuClock)
    {
        // We read keyboard if we're reading the ULA port, during TW.
        if ((~z80_c & ~z80_c_d & SIGNAL_IORQ_) == SIGNAL_IORQ_)
        {
            if ((z80_a & 0x0001) == 0x0000)
            {
                if ((z80_c & SIGNAL_RD_) == 0x0000)
                {
                    ioPortIn |= 0xBF;
                    if ((z80_a & 0x8000) == 0x0000) ioPortIn &= keys[0];
                    if ((z80_a & 0x4000) == 0x0000) ioPortIn &= keys[1];
                    if ((z80_a & 0x2000) == 0x0000) ioPortIn &= keys[2];
                    if ((z80_a & 0x1000) == 0x0000) ioPortIn &= keys[3];
                    if ((z80_a & 0x0800) == 0x0000) ioPortIn &= keys[4];
                    if ((z80_a & 0x0400) == 0x0000) ioPortIn &= keys[5];
                    if ((z80_a & 0x0200) == 0x0000) ioPortIn &= keys[6];
                    if ((z80_a & 0x0100) == 0x0000) ioPortIn &= keys[7];
                    d_o = ioPortIn;
                }

                if ((z80_c & SIGNAL_WR_) == 0x0000)
                {
                    ioPortOut = d_i;
                }
            }
        }
        z80_c_d = z80_c;
    }

    d = d_o;

    // 4. Generate video signal.
    {
        // 4.a. Generate colours.
        rgba = colourTable[((data & 0x80) ^ (attr & flash)) | (attr & 0x7F)];

        // 4.b. Update data and attribute registers.
        data <<= 1;

        // We start outputting data after just a data/attr pair has been fetched.
        if ((pixel & 0x07) == 0x03)
        {
            // This actually causes the following, due to the placement of the
            // aliases:
            // data = dataOut; attr = attrOut;
            // dataOut = dataLatch; attrOut = attrLatch;
            // attrLatch = ioPortOut; dataLatch = 0xFF;
            dataReg <<= 8;
            attrReg <<= 8;
            dataLatch = 0xFF;
            attrLatch = ioPortOut & 0x07;
        }
    }

    // 5. Update counters
    if (++pixel == maxPixel)
    {
        pixel = 0;
        ++scan;
        if (scan == vBlankEnd)
            flash += 0x04;
        else if (scan == maxScan)
            scan = 0;
    }
}
// vim: et:sw=4:ts=4:
