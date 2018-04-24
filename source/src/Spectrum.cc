#include "Spectrum.h"

#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    bus(0xFF),
    joystick(0),
    kempston(false),
    spectrum128K(false),
    spectrumPlus2(false),
    spectrumPlus2A(false),
    idle(0xFF),
    paging(0x0020), mask(0x0001),
    contendedPage{false, true, false, false},
    romPage{true, false, false, false},
    set48(true), rom48(true)
{
    buzzer.init(&ula.ioPortOut, &ula.tapeIn);

    // This is just for the laughs. We initialize the whole RAM to random
    // values to see the random attributes that appeared in the Spectrum
    // at boot time.
    srand(static_cast<unsigned int>(time(0)));
    for (size_t a = 0; a < (2 << 17); ++a)
        ram[a] = rand() & 0xFF;

    setPage(0, 0, true, false);
    setPage(1, 5, false, true);
    setPage(2, 2, false, false);
    setPage(3, 0, false, false);
    setScreen(5);
}

void Spectrum::loadRoms(size_t model)
{
    ifstream ifs;
    vector<string> romNames;
    vector<string> romPaths;
    string romName;

    char* pHome = getenv(SPECIDE_HOME_ENV);

    romPaths.push_back("");
    if (pHome != nullptr)
    {
#if (SPECIDE_ON_UNIX==1)
        string home(pHome);
        home += string("/") + string(SPECIDE_CONF_DIR) + string("/roms/");
        romPaths.push_back(home);
#else
        string home(pHome);
        home += string("\\") + string(SPECIDE_CONF_DIR) + string("\\roms\\");
        romPaths.push_back(home);
#endif
    }
#if (SPECIDE_ON_UNIX==1)
    romPaths.push_back("/usr/local/share/spectrum-roms/");
    romPaths.push_back("/usr/share/spectrum-roms/");
#endif

    switch (model)
    {
        case 0:
            romNames.push_back("48.rom");
            break;

        case 1:
            romNames.push_back("128-0.rom");
            romNames.push_back("128-1.rom");
            break;

        case 2:
            romNames.push_back("plus2-0.rom");
            romNames.push_back("plus2-1.rom");
            break;

        case 3:
            romNames.push_back("plus3-0.rom");
            romNames.push_back("plus3-1.rom");
            romNames.push_back("plus3-2.rom");
            romNames.push_back("plus3-3.rom");
            break;

        case 4:
            romNames.push_back("128-spanish-0.rom");
            romNames.push_back("128-spanish-1.rom");
            break;

        default:
            romNames.push_back("48.rom");
            break;
    }

    size_t j = 0;
    bool fail = true;

    do
    {
        for (size_t i = 0; i < romNames.size(); ++i)
        {
            size_t pos = 0;

            romName = romPaths[j] + romNames[i];
            printf("Trying ROM: %s\n", romName.c_str());
            ifs.open(romName, ifstream::binary);

            // If it fails, try the ROM in /usr/share/spectrum-roms
            fail = ifs.fail();
            if (fail)
                break;

            char c;
            while (ifs.get(c))
                rom[((2 << 14) * i) + pos++] = c;

            ifs.close();
        }
        ++j;
    } while (fail && j < romPaths.size());
}

void Spectrum::set128K()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2()
{
    spectrum128K = true;
    spectrumPlus2 = true;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2A()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = false;
    mask = 0x0004;
    reset();
}

void Spectrum::setPlus3()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = true;
    mask = 0x0004;
    reset();
}

void Spectrum::clock()
{
    // ULA is 'clocked' before Z80. This means:
    //
    // ULA   Z80
    // ---------
    //  0  | RST
    //  1  |  H
    //  2  |  L
    //  3  |  H
    //  4  |  L
    //
    // This is important, because when ULA is at 0 again Z80 clock will be
    // LOW. Contention intervals work this way.
    bool as_ = ((z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_);
    bool io_ = ((z80.c & SIGNAL_IORQ_) == SIGNAL_IORQ_);
    bool rd_ = ((z80.c & SIGNAL_RD_) == SIGNAL_RD_);
    bool wr_ = ((z80.c & SIGNAL_WR_) == SIGNAL_WR_);
    bool rf_ = ((z80.c & SIGNAL_RFSH_) == SIGNAL_RFSH_);
    size_t memArea = (z80.a & 0xC000) >> 14;
    bool snow = (!spectrumPlus2A)
        && (as_ == true && rf_ == false && contendedPage[memArea] == true)
        && ((ula.a & 0x1800) != 0x1800)
        && ((ula.a & 0x0001) != 0x0001);

    static uint_fast8_t count = 0;

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

    // If a contended RAM page is selected, we'll have memory contention.
    ula.contendedBank = contendedPage[memArea];

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    // I've found that separating both data buses is helpful for all
    // Speccies.
    ula.io = z80.d;
    ula.d = scr[snow ? (ula.a & 0x3F00) | z80.ir.l : ula.a];
    if (ula.mem == false)
        bus = ula.d;

    ula.clock();
    z80.c = ula.z80_c;

    if ((++count & 0x03) == 0x00)
    {
        count = 0;
        buzzer.update();
        if (spectrum128K) psg.clock();
    }

    // We clock the Z80 if the ULA allows.
    if (ula.cpuClock)
    {
        // Z80 gets data from the ULA or memory, only when reading.
        if (io_ == false)
        {
            if (rd_ == false)
            {
                if (kempston == true && ((z80.a & 0x00E0) == 0x0000))  // Kempston joystick.
                {
                    z80.d = joystick;
                }
                else if ((z80.a & 0x0001) == 0x0000)
                {
                    z80.d = ula.io;
                }
                else if (ula.idle == false)
                {
                    z80.d = bus & idle;  // Get the byte from the video memory.
                }
                else
                {
                    z80.d = idle;
                }
            }

            if (spectrum128K)
            {
                switch (z80.a & 0xF003)
                {
                    case 0x0001:    // In +2A/+3 this is the floating bus port.
                        if (spectrumPlus2A)
                        {
                            if (rd_ == false)
                            {
                                if ((paging & 0x0020) == 0x0000)
                                    z80.d = (bus_1 & idle) | 0x01;
                            }
                            break;
                        }
                        // fall-through
                    case 0x1001:    // 0x1FFD (+3 Paging High)
                        if (spectrumPlus2A)
                        {
                            if (wr_ == false)
                                updatePage(1);
                            break;
                        }
                        // fall-through
                    case 0x2001:    // 0x2FFD (+3 FDC Main Status)
                        if (spectrumPlus2A)
                        {
                            break;
                        }
                        // fall-through
                    case 0x3001:    // 0x3FFD (+3 FDC Data)
                        if (spectrumPlus2A)
                        {
                            break;
                        }
                        // fall-through
                    case 0x4001: // fall-through
                    case 0x5001: // fall-through
                    case 0x6001: // fall-through
                    case 0x7001: // 0x7FFD (128K Paging / +3 Paging Low)
                        if ((wr_ == false) || (spectrumPlus2A == false && rd_ == false))
                            updatePage(0);
                        break;

                    case 0x8001: // fall-through
                    case 0x9001: // fall-through
                    case 0xA001: // fall-through
                    case 0xB001: // 0xBFFD
                        if (wr_ == false)
                            psg.write(z80.d);
                        break;

                    case 0xC001: // fall-through
                    case 0xD001: // fall-through
                    case 0xE001: // fall-through
                    case 0xF001: // 0xFFFD
                        if (wr_ == false)
                            psg.addr(z80.d);
                        else if (rd_ == false)
                            z80.d = psg.read();
                        break;
                }
            }
        }
        else if (as_ == false)
        {
            // Bank 0: 0000h - ROM
            // Bank 1: 4000h - Contended memory
            // Bank 2: 8000h - Extended memory
            // Bank 3: C000h - Extended memory (can be contended)
            if (rd_ == false)
            {
                z80.d = map[memArea][z80.a & 0x3FFF];
                if (contendedPage[memArea] == true && ula.mem == true)
                    bus = z80.d;
            }
            else if (romPage[memArea] == false && wr_ == false)
                if (z80.state == Z80State::ST_MEMWR_T3L_DATAWR)
                    map[memArea][z80.a & 0x3FFF] = z80.d;
        }
        else
        {
            z80.d = 0xFF;
        }

        z80.clock();
    }

    bus_1 = bus;
}

void Spectrum::updatePage(uint_fast8_t reg)
{
    static size_t wrWait = 0;

    ++wrWait;
    if (wrWait == 5)
    {
        wrWait = 0;
        if ((paging & 0x0020) == 0x0000)
        {
            if (reg == 1)
                paging = (z80.d << 8) | (paging & 0x00FF);
            else
                paging = z80.d | (paging & 0xFF00);

            if ((paging & 0x0100) == 0x0100)    // Special paging mode.
            {
                switch (paging & 0x0600)
                {
                    case 0x0000:
                        setPage(0, 0, false, false);
                        setPage(1, 1, false, false);
                        setPage(2, 2, false, false);
                        setPage(3, 3, false, false);
                        break;
                    case 0x0200:
                        setPage(0, 4, false, true);
                        setPage(1, 5, false, true);
                        setPage(2, 6, false, true);
                        setPage(3, 7, false, true);
                        break;
                    case 0x0400:
                        setPage(0, 4, false, true);
                        setPage(1, 5, false, true);
                        setPage(2, 6, false, true);
                        setPage(3, 3, false, false);
                        break;
                    case 0x0600:
                        setPage(0, 4, false, true);
                        setPage(1, 7, false, true);
                        setPage(2, 6, false, true);
                        setPage(3, 3, false, false);
                        break;
                }
            }
            else                                // Normal paging mode.
            {
                size_t ramBank = paging & 0x0007;
                size_t romBank =
                    ((paging & 0x0010) >> 4) | ((paging & 0x0400) >> 9);

                setScreen(((paging & 0x0008) >> 2) | 0x05);

                setPage(0, romBank, true, false);
                setPage(1, 5, false, true);
                setPage(2, 2, false, false);
                setPage(3, ramBank, false, ((paging & mask) == mask));

                rom48 = ((spectrumPlus2A && romBank == 3)
                    || (spectrum128K && romBank == 1)) ? true : false;
                set48 = ((paging & 0x0020) == 0x0020);
            }
        }
    }
}

void Spectrum::reset()
{
    ula.reset();    // Synchronize clock level.
    z80.reset();
    psg.reset();

    if (spectrum128K)
    {
        setPage(0, 0, true, false);
        setPage(1, 5, false, true);
        setPage(2, 2, false, false);
        setPage(3, 0, false, false);
        setScreen(5);
        paging = 0x0000;
        set48 = false;
        rom48 = false;
    }
    else
    {
        setPage(0, 0, true, false);
        setPage(1, 5, false, true);
        setPage(2, 2, false, false);
        setPage(3, 0, false, false);
        setScreen(5);
        paging = 0x0020;
        set48 = true;
        set48 = true;
    }
}

// vim: et:sw=4:ts=4
