#include "Spectrum.h"
#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    joystick(0),
    kempston(false),
    spectrum128K(false),
    spectrumPlus2(false),
    spectrumPlus2A(false),
    idle(0xFF),
    paging(0x0020),
    screen(5),
    contendedPage{false, true, false, false},
    ram{Memory(14), Memory(14), Memory(14), Memory(14),     // 64K
        Memory(14), Memory(14), Memory(14), Memory(14)},    // 128K
    rom{Memory(14, true), Memory(14, true),     // 32K
        Memory(14, true), Memory(14, true)},    // 64K
    map{&rom[0], &ram[5], &ram[2], &ram[0]}
{
    buzzer.init(&ula.ioPortOut, &ula.tapeIn);

    // This is just for the laughs. We initialize the whole RAM to random
    // values to see the random attributes that appeared in the Spectrum
    // at boot time.
    srand(static_cast<unsigned int>(time(0)));
    for (size_t a = 0; a < 0x4000; ++a)
    {
        map[1]->memory[a] = rand() & 0xFF;
        map[2]->memory[a] = rand() & 0xFF;
        map[3]->memory[a] = rand() & 0xFF;
    }
}

void Spectrum::loadRoms(size_t model)
{
    size_t pos = 0;
    char c;

    ifstream ifs;
    string romPath("/usr/share/spectrum-roms/");
    vector<string> romNames;
    string romName;

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
    }

    for (size_t i = 0; i < romNames.size(); ++i)
    {
        romName = romNames[i];
        pos = 0;

        // Try opening ROM in the current directory.
        printf("Trying ROM: %s\n", romName.c_str());
        ifs.open(romName, ifstream::binary);

        // If it fails, try the ROM in /usr/share/spectrum-roms
        if (ifs.fail())
        {
            romName = romPath + romName;
            printf("Trying ROM: %s\n", romName.c_str());
            ifs.open(romName, ifstream::binary);
        }

        while (ifs.get(c))
            rom[i].memory[pos++] = c;

        ifs.close();
    }
}

void Spectrum::set128K()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    reset();
}

void Spectrum::setPlus2()
{
    spectrum128K = true;
    spectrumPlus2 = true;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    reset();
}

void Spectrum::setPlus2A()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = false;
    reset();
}

void Spectrum::setPlus3()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = true;
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
    size_t memArea = (z80.a & 0xC000) >> 14;

    static size_t count = 0;

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

    // If a contended RAM page is selected, we'll have memory contention.
    if ((contendedPage[memArea] == true))
        ula.contendedBank = true;
    else
        ula.contendedBank = false;

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    if (!spectrumPlus2A)
    {
        if (ula.hiz == false)           // Is ULA mastering the bus?
        {

            // ULA renders the selected memory bank for video.
            // In 48K models, this one is fixed.
            // In 128K models, this one can be RAM5 or RAM7.
            ula.d = ram[screen].read(ula.a);
        }
        else    // If ULA is not mastering, Z80 is.
        {
            // ula.d = z80.d;
            ula.io = z80.d;
        }
    }
    else
    {
        ula.d = ram[screen].read(ula.a);
        ula.io = z80.d;
    }

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
                    z80.d = ram[screen].d & idle;  // Get the byte from the video memory.
                }
                else
                {
                    z80.d = idle;
                }
            }

            if (spectrum128K)
            {
                if (spectrumPlus2A)
                {
                    if ((z80.a & 0xC002) == 0x4000)         // 0x7FFD.
                    {
                        if (wr_ == false)
                            updatePagePlus2A(0);
                    }
                    else if ((z80.a & 0xF002) == 0x1000)    // 0x1FFD.
                    {
                        if (wr_ == false)
                            updatePagePlus2A(1);
                    }
                    else if ((z80.a & 0xC002) == 0x8000)    // 0xBFFD: AY Data.
                    {
                        if (wr_ == false)
                        {
                            psg.write(z80.d);
                        }
                    }
                    else if ((z80.a & 0xC002) == 0xC000)    // 0xFFFD: AY Regs.
                    {
                        if (wr_ == false)
                        {
                            psg.addr(z80.d);
                        }
                        else if (rd_ == false)
                        {
                            z80.d = psg.read();
                        }
                    }
                }
                else
                {
                    if ((z80.a & 0x8002) == 0x0000)         // 0x7FFD: Page.
                    {
                        if (wr_ == false || rd_ == false)
                            updatePage128K();
                    }
                    else if ((z80.a & 0xC002) == 0x8000)    // 0xBFFD: AY Data.
                    {
                        if (wr_ == false)
                        {
                            psg.write(z80.d);
                        }
                    }
                    else if ((z80.a & 0xC002) == 0xC000)    // 0xFFFD: AY Regs.
                    {
                        if (wr_ == false)
                        {
                            psg.addr(z80.d);
                        }
                        else if (rd_ == false)
                        {
                            z80.d = psg.read();
                        }
                    }
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
                z80.d = map[memArea]->read(z80.a);
            else if (wr_ == false)
                map[memArea]->write(z80.a, z80.d);
        }
        else
        {
            z80.d = 0xFF;
        }

        z80.clock();
    }
}

void Spectrum::updatePage128K()
{
    static size_t wrWait = 0;

    ++wrWait;
    if (wrWait == 5)
    {
        wrWait = 0;
        if ((paging & 0x0020) == 0x0000)
        {
            paging = z80.d;

            size_t ramBank = paging & 0x0007;
            size_t romBank = (paging & 0x0010) >> 4;
            screen = ((paging & 0x0008) >> 2) | 0x05;

            map[0] = &rom[romBank];
            map[1] = &ram[5];
            map[2] = &ram[2];
            map[3] = &ram[ramBank];

            contendedPage[0] = false;
            contendedPage[1] = true;
            contendedPage[2] = false;
            contendedPage[3] = ((paging & 0x0001) == 0x01);   // RAM 1-3-5-7
        }
    }
}

void Spectrum::updatePagePlus2A(uint_fast8_t reg)
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
                        map[0] = &ram[0]; contendedPage[0] = false;
                        map[1] = &ram[1]; contendedPage[1] = false;
                        map[2] = &ram[2]; contendedPage[2] = false;
                        map[3] = &ram[3]; contendedPage[3] = false;
                        break;
                    case 0x0200:
                        map[0] = &ram[4]; contendedPage[0] = true;
                        map[1] = &ram[5]; contendedPage[1] = true;
                        map[2] = &ram[6]; contendedPage[2] = true;
                        map[3] = &ram[7]; contendedPage[3] = true;
                        break;
                    case 0x0400:
                        map[0] = &ram[4]; contendedPage[0] = true;
                        map[1] = &ram[5]; contendedPage[1] = true;
                        map[2] = &ram[6]; contendedPage[2] = true;
                        map[3] = &ram[3]; contendedPage[3] = false;
                        break;
                    case 0x0600:
                        map[0] = &ram[4]; contendedPage[0] = true;
                        map[1] = &ram[7]; contendedPage[1] = true;
                        map[2] = &ram[6]; contendedPage[2] = true;
                        map[3] = &ram[3]; contendedPage[3] = false;
                        break;
                }
            }
            else                                // Normal paging mode.
            {
                size_t ramBank = paging & 0x0007;
                size_t romBank =
                    ((paging & 0x0010) >> 4) | ((paging & 0x0400) >> 9);
                screen = ((paging & 0x0008) >> 2) | 0x05;
                // screen = ((paging & 0x08) == 0x00) ? 5 : 7;

                map[0] = &rom[romBank];
                map[1] = &ram[5];
                map[2] = &ram[2];
                map[3] = &ram[ramBank];

                contendedPage[0] = false;
                contendedPage[1] = true;
                contendedPage[2] = false;
                contendedPage[3] = ((paging & 0x0004) == 0x0004);   // RAM 4-5-6-7
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
        map[0] = &rom[0]; contendedPage[0] = false;
        map[1] = &ram[5]; contendedPage[1] = true;
        map[2] = &ram[2]; contendedPage[2] = false;
        map[3] = &ram[0]; contendedPage[3] = false;

        screen = 5;
        paging = 0x0000;
    }
    else
    {
        map[0] = &rom[0]; contendedPage[0] = false;
        map[1] = &ram[5]; contendedPage[1] = true;
        map[2] = &ram[2]; contendedPage[2] = false;
        map[3] = &ram[0]; contendedPage[3] = false;

        screen = 5;
        paging = 0x0020;
    }
}

// vim: et:sw=4:ts=4
