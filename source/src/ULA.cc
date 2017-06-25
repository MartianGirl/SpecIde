#include "ULA.h"

ULA::ULA() :
    hiz(true),
    z80_a(0xFFFF), z80_c(0xFFFF), z80_c_delayed(0xFFFF),
    contentionWindow(false),
    memContention(false), ioContention(false),
    cpuWait(false), cpuClock(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
    flash(0),
    dataReg(0), attrReg(0),
#if SPECIDE_BYTE_ORDER == 1
    data(*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint_fast32_t) - 3)),
    attr(*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint_fast32_t) - 3)),
    dataLatch(*(reinterpret_cast<uint8_t*>(&dataReg) + sizeof(uint_fast32_t) - 1)),
    attrLatch(*(reinterpret_cast<uint8_t*>(&attrReg) + sizeof(uint_fast32_t) - 1)),
#else
    data(*(reinterpret_cast<uint8_t*>(&dataReg) + 2)),
    attr(*(reinterpret_cast<uint8_t*>(&attrReg) + 2)),
    dataLatch(*(reinterpret_cast<uint8_t*>(&dataReg) + 0)),
    attrLatch(*(reinterpret_cast<uint8_t*>(&attrReg) + 0)),
#endif
    hBlank(false), vBlank(false), display(false),
    pixelStart(0x000), pixelEnd(0x0FF),
    hBorderStart(0x100), hBorderEnd(0x1BF),
    hBlankStart(0x140), hBlankEnd(0x19F),
    // hSyncStart(0x150), hSyncEnd(0x16F),     // ULA 5C (Issue 2)
    hSyncStart(0x158), hSyncEnd(0x177),     // ULA 6C (Issue 3)
    scanStart(0x000), scanEnd(0x0BF),
    vBorderStart(0x0C0), vBorderEnd(0x137),
    vBlankStart(0x0F8), vBlankEnd(0x0FF),
    vSyncStart(0x0F8), vSyncEnd(0x0FB),
    ioPortIn(0xFF), ioPortOut(0x00), capacitor(0),
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
    // Here we:
    // 1. Generate video control signals.
    hSync = (pixel >= hSyncStart) && (pixel <= hSyncEnd);
    vSync = ((scan & 0x0FC) == 0x0F8);
    // vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);

    vBlank = ((scan & 0x0F8) == 0x0F8);
    hBlank = ((pixel >= hBlankStart) && (pixel <= hBlankEnd));
    display = (pixel < 0x100) && (scan < 0x0C0);

    // 2. Generate video data.
    if (display)
    {
        // 2.a. Check for contended memory or I/O accesses.
        // T1 of a memory cycle can be detected by a falling edge on MREQ.
        // T2 of an I/O cycle can be detected by a falling edge on IORQ.
        memContention = ((z80_a & 0xC000) == 0x4000)
            && ((~z80_c & z80_c_delayed & SIGNAL_MREQ_) == SIGNAL_MREQ_);
        ioContention = ((z80_a & 0x0001) == 0x0000) 
            && ((~z80_c & z80_c_delayed & SIGNAL_IORQ_) == SIGNAL_IORQ_);

        // 2.b. Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
                contentionWindow = false; hiz = true; break;
            case 0x04:
            case 0x05:
            case 0x06:
                contentionWindow = true; hiz = true; break;
            case 0x07:
                // Generate addresses (which must be pair).
                dataAddr = ((pixel & 0xF0) >> 3)    // 000SSSSS SSSPPPP0
                    | ((scan & 0x38) << 2)          // 00076210 54376540
                    | ((scan & 0x07) << 8)
                    | ((scan & 0xC0) << 5);

                attrAddr = ((pixel & 0xF0) >> 3)    // 000110SS SSSPPPP0
                    | ((scan & 0xF8) << 2)          // 00000076 54376540
                    | 0x1800;
                contentionWindow = true; hiz = true; break;
            case 0x08:
                a = dataAddr;
                contentionWindow = true; hiz = false; break;
            case 0x09:
                dataLatch = d;
                contentionWindow = true; hiz = false; break;
            case 0x0A:
                a = attrAddr;
                contentionWindow = true; hiz = false; break;
            case 0x0B:
                attrLatch = d;
                contentionWindow = true; hiz = false; break;
            case 0x0C:
                a = dataAddr + 1;
                contentionWindow = true; hiz = false; break;
            case 0x0D:
                dataLatch = d;
                contentionWindow = true; hiz = false; break;
            case 0x0E:
                a = attrAddr + 1;
                contentionWindow = true; hiz = false; break;
            case 0x0F:
                attrLatch = d;
                contentionWindow = true; hiz = false; break;
            default:
                a = 0xFFFF;
                contentionWindow = false; hiz = true; break;
        }

        // 2.c. Resolve contention and generate CPU clock.
        cpuWait = contentionWindow && (memContention || ioContention);
    }
    else
    {
        hiz = true;
        cpuWait = false;
    }
    
    cpuClock = !cpuWait && ((pixel & 0x0001) == 0x0000);

    // 3. ULA port & Interrupt.
    c = z80_c;
    if ((scan == vSyncStart) && (pixel < 64))
        c &= ~SIGNAL_INT_;
    else
        c |= SIGNAL_INT_;

    if (cpuClock)
    {
        // We read keyboard if we're reading the ULA port, during TW.
        if (((z80_a & 0x0001) == 0x0000)
                && ((~z80_c & ~z80_c_delayed & SIGNAL_IORQ_) == SIGNAL_IORQ_))
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
                d = ioPortIn;
            }

            if ((z80_c & SIGNAL_WR_) == 0x0000)
            {
                ioPortOut = d;
            }
        }
        z80_c_delayed = z80_c;
    }

    // First attempt at MIC/EAR feedback loop.
    // Let's keep this here for the moment.
    capacitor += ((ioPortOut & 0x10) == 0x00) ? -1 : 11; // Issue 3
    // capacitor += ((ioPortOut & 0x18) == 0x00) ? -1 : 11; // Issue 2
    if (capacitor < 0) capacitor = 0;
    if (capacitor > 5600) capacitor = 5600;
                
    ioPortIn &= 0xBF;
    ioPortIn |= (capacitor == 0) ? 0x00 : 0x40;

    // 4. Generate video signal.
    if (!(hBlank || vBlank))
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
    pixel = (pixel + 1) % maxPixel;
    if (pixel == hSyncStart)
    {
        scan = (scan + 1) % maxScan;
        if (scan == vBlankEnd)
            flash += 0x04;
    }
}
// vim: et:sw=4:ts=4:
