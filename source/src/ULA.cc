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
    blank(false), border(false),
    pixelStart(0), pixelEnd(255),
    hBorderStart(256), hBorderEnd(447),
    hBlankStart(320), hBlankEnd(415),
    hSyncStart(344), hSyncEnd(375),
    scanStart(0), scanEnd(191),
    vBorderStart(192), vBorderEnd(311),
    vBlankStart(248), vBlankEnd(255),
    vSyncStart(248), vSyncEnd(251),
    ioPortIn(0x00), ioPortOut(0x00), ulaRead(false),
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
    // 1.a. Generate video control signals.
    hSync = (pixel >= hSyncStart) && (pixel <= hSyncEnd);
    vSync = (scan >= vSyncStart) && (scan <= vSyncEnd);
    
    blank = ((pixel >= hBlankStart) && (pixel <= hBlankEnd))
        || ((scan >= vBlankStart) && (scan <= vBlankEnd));

    border = ((pixel >= hBorderStart) && (pixel <= hBorderEnd))
        || ((scan >= vBorderStart) && (scan <= vBorderEnd));

    // 1.b. Check for contended memory or I/O accesses.
    memContention = ((z80_a & 0xC000) == 0x4000)
        && ((z80_c & SIGNAL_MREQ_) == 0x00) 
        && ((z80_c_1d & SIGNAL_MREQ_) == SIGNAL_MREQ_); // MREQ edge means T1.
    ioContention = ((z80_a & 0x0001) == 0x0000) 
        && ((z80_c & SIGNAL_IORQ_) == 0x00)
        && ((z80_c_1d & SIGNAL_IORQ_) == SIGNAL_IORQ_); // IORQ edge means T2.

    // 2. Generate video data.
    if (!border)
    {
        switch (pixel & 0x0F)
        {
            case 0x00:
                contentionWindow = false;
                a = 0xFFFF; hiz = true; break;
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
                dataLatch = d; break;
            case 0x0A:
                a = attrAddr; break;
            case 0x0B:
                attrLatch = d; break;
            case 0x0C:
                a = dataAddr + 1; break;
            case 0x0D:
                dataLatch = d; break;
            case 0x0E:
                a = attrAddr + 1; break;
            case 0x0F:
                attrLatch = d; break;
            default:
                a = 0xFFFF; rd_ = true; break;
        }
    }

    // 2.b Resolve contention and generate CPU clock.
    cpuWait = (contentionWindow && (cpuWait || memContention || ioContention));
    cpuClock = ((pixel & 0x0001) == 0x0000) && !cpuWait;
    if (!cpuWait)
        z80_c_1d = z80_c;

    // 2.c ULA port.
    if (((z80_a & 0x0001) == 0x0000) 
        && ((z80_c & SIGNAL_IORQ_) == 0x00)         // Can be T2 or TW
        && ((z80_c_1d & SIGNAL_IORQ_) == 0x00))     // Only in TW
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

    if ((z80_c & SIGNAL_IORQ_) == SIGNAL_IORQ_)
        ulaRead = false;


    // 2.d Interrupt.
    c = z80_c;
    if (intCounter)
    {
        c &= ~SIGNAL_INT_;
        --intCounter;
    }
    else
    {
        c |= SIGNAL_INT_;
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
        if (scan == vSyncStart)
            intCounter = 64;
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
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x0400) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) ? 0xFD: 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x0200) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) ? 0xFE : 0xFF;
    }

    if ((z80_a & 0x0100) == 0x0000)
    {
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? 0xEF : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) ? 0xF7 : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) ? 0xFB : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) ? 0xFD : 0xFF;
        ioPortIn &= (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) ? 0xFE : 0xFF;
    }
}

// vim: et:sw=4:ts=4:
