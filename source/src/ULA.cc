#include "ULA.h"

ULA::ULA() :
    hiz(true),
    as_(*(&hiz)), rd_(*(&hiz)), // Just some aliases.
    z80_a(0xFFFF), z80_c(0xFFFF), z80_c_1d(0xFFFF),
    contentionWindow(false),
    memContention(false), ioContention(false),
    cpuWait(false), cpuClock(false),
    scan(0), maxScan(312),      // PAL values. Must make this configurable.
    pixel(0), maxPixel(448),
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
    blank(false), display(false),
    pixelStart(0x000), pixelEnd(0x0FF),
    hBorderStart(0x100), hBorderEnd(0x1BF),
    hBlankStart(0x140), hBlankEnd(0x19F),
    hSyncStart(0x158), hSyncEnd(0x177),
    scanStart(0x000), scanEnd(0x0BF),
    vBorderStart(0x0C0), vBorderEnd(0x137),
    vBlankStart(0x0F8), vBlankEnd(0x0FF),
    vSyncStart(0x0F8), vSyncEnd(0x0FB),
    ioPortIn(0xFF), ioPortOut(0x00), ulaRead(false),
    c(0xFFFF), intCounter(0)
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

    blank = ((scan & 0x0F8) == 0x0F8)
        || ((pixel >= hBlankStart) && (pixel <= hBlankEnd));
    display = (pixel < 0x100) && (scan < 0x0C0);

    // 2. Generate video data.
    if (display)
    {
        // 2.a. Check for contended memory or I/O accesses.
        // T1 of a memory cycle can be detected by a falling edge on MREQ.
        // T2 of an I/O cycle can be detected by a falling edge on IORQ.
        memContention = ((z80_a & 0xC000) == 0x4000)
            && ((~z80_c & z80_c_1d & SIGNAL_MREQ_) == SIGNAL_MREQ_);
        ioContention = ((z80_a & 0x0001) == 0x0000) 
            && ((~z80_c & z80_c_1d & SIGNAL_IORQ_) == SIGNAL_IORQ_);

        // 2.b. Read from memory.
        switch (pixel & 0x0F)
        {
            case 0x00:
                contentionWindow = false; break;
            case 0x01:
                break;
            case 0x02:
                break;
            case 0x03:
                break;
            case 0x04:
                contentionWindow = true; break;
            case 0x05:
                break;
            case 0x06:
                break;
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
                a = dataAddr; hiz = false; break;
            case 0x09:
                dataLatch = d; hiz = true; break;
            case 0x0A:
                a = attrAddr; hiz = false; break;
            case 0x0B:
                attrLatch = d; hiz = true; break;
            case 0x0C:
                a = dataAddr + 1; hiz = false; break;
            case 0x0D:
                dataLatch = d; hiz = true; break;
            case 0x0E:
                a = attrAddr + 1; hiz = false; break;
            case 0x0F:
                attrLatch = d; hiz = true; break;
            default:
                a = 0xFFFF; rd_ = true; break;
        }

        // 2.c. Resolve contention and generate CPU clock.
        cpuWait = contentionWindow && (memContention || ioContention);
    }
    else
    {
        cpuWait = false;
    }
    
    cpuClock = !cpuWait && ((pixel & 0x0001) == 0x0000);

    // 3. ULA port.
    if (cpuClock)
    {
        ulaRead = false;
        if (((z80_a & 0x0001) == 0x0000) 
                && ((~z80_c & ~z80_c_1d & SIGNAL_IORQ_) == SIGNAL_IORQ_))   // Only in TW
        {
            if ((z80_c & SIGNAL_RD_) == 0x0000)
            {
                ulaRead = true;
                readKeys();
                d = ioPortIn;
            }
            if ((z80_c & SIGNAL_WR_) == 0x0000)
                ioPortOut = d;
        }
        z80_c_1d = z80_c;
    }

    // 4. Interrupt.
    c = z80_c;
    if (scan == vSyncStart && pixel < 64)
        c &= ~SIGNAL_INT_;
    else
        c |= SIGNAL_INT_;

    // 5. Generate video signal.
    if (!blank)
    {
        // 5.a. Generate colours.
        rgba = colourTable[((data & 0x80) ^ (attr & flash)) | (attr & 0x7F)];

        // 5.b. Update data and attribute registers.
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

    // 6. Update counters
    pixel = (pixel + 1) % maxPixel;
    if (pixel == hSyncStart)
    {
        scan = (scan + 1) % maxScan;
        if (scan == vBlankEnd)
            flash += 0x04;
    }
}

void ULA::readKeys()
{
    ioPortIn |= 0x1F;
    if ((z80_a & 0x8000) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x4000) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) ? 0xFE : 0xFF;
    }
    
    if ((z80_a & 0x2000) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x1000) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x0800) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) ? 0xFE : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) ? 0xEF : 0xFF;
    }

    if ((z80_a & 0x0400) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ? 0xFE : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) ? 0xF7: 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) ? 0xEF : 0xFF;
    }

    if ((z80_a & 0x0200) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? 0xFE : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) ? 0xEF : 0xFF;
    }

    if ((z80_a & 0x0100) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 0xFE : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) ? 0xEF : 0xFF;
    }
}

// vim: et:sw=4:ts=4:
