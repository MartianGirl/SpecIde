#include "Spectrum.h"
#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    joystick(0),
    kempston(true),
    spectrum128K(false),
    idle(0xFF),
    paging(0x20),
    ramBank(3), romBank(0), scrBank(5),
    contendedRam(false), contendedRom(false),
    ram{Memory(14), Memory(14), Memory(14), Memory(14),     // 64K
        Memory(14), Memory(14), Memory(14), Memory(14),     // 128K
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),     // 256K
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),     // 512K
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14),
        Memory(14), Memory(14), Memory(14), Memory(14)},    // 1024K
    rom{Memory(14, true), Memory(14, true), Memory(14, true), Memory(14, true),
        Memory(14, true), Memory(14, true), Memory(14, true), Memory(14, true),
        Memory(14, true), Memory(14, true), Memory(14, true), Memory(14, true),
        Memory(14, true), Memory(14, true), Memory(14, true), Memory(14, true)},
    map{&rom[0], &ram[5], &ram[2], &ram[0]}
{
    // This is just for the laughs. We initialize the whole RAM to random
    // values to see the random attributes that appeared in the Spectrum
    // at boot time.
    srand(static_cast<unsigned int>(time(0)));
    for (size_t a = 0; a < 0x4000; ++a)
    {
        map[1]->memory[a] = rand() % 0x100;
        map[2]->memory[a] = rand() % 0x100;
        map[3]->memory[a] = rand() % 0x100;
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
    }

    for (size_t i = 0; i < romNames.size(); ++i)
    {
        romName = romNames[i];
        pos = 0;

        // Try opening ROM in the current directory.
        cout << "Trying ROM: " << romName << endl;
        ifs.open(romName, ifstream::binary);

        // If it fails, try the ROM in /usr/share/spectrum-roms
        if (ifs.fail())
        {
            romName = romPath + romName;
            cout << "Trying ROM: " << romName << endl;
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
    reset();
}

void Spectrum::setPlus2()
{
    spectrum128K = true;
    spectrumPlus2 = true;
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
    ++count;

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

    // If a contended RAM page is selected, we'll have memory contention:
    // - If the address is in the C000h-FFFFh range, and it is odd.
    if ((contendedRam == true) && ((z80.a & 0xC001) == 0xC001))
        ula.z80_mask = 0x7FFF;
    else
        ula.z80_mask = 0xFFFF;

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    if (ula.hiz == false)           // Is ULA mastering the bus?
    {

        // ULA renders the selected memory bank for video.
        // In 48K models, this one is fixed.
        // In 128K models, this one can be RAM5 or RAM7.
        ula.d = ram[scrBank].read(ula.a);
    }
    else    // If ULA is not mastering, Z80 is.
    {
        ula.d = z80.d;
    }

    ula.clock();
    z80.c = ula.c;

    if (spectrum128K && ((count & 0x03) == 0x00))
    {
        psg.clock();
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
                    z80.d = ula.d;
                }
                else if (ula.idle == false)
                {
                    z80.d = ram[scrBank].d & idle;  // Get the byte from the video memory.
                }
                else
                {
                    z80.d = idle;
                }
            }

            if (spectrum128K)
            {
                uint_fast16_t portDecode128K = z80.a & 0xC002;
                if ((portDecode128K & 0x8002) == 0x0000)
                {
                    if (wr_ == false || rd_ == false)
                        updatePage();
                }
                else if (portDecode128K == 0x8000)  // 0xBFFD: AY Data.
                {
                    if (wr_ == false)
                    {
                        psg.write(z80.d);
                    }
                }
                else if (portDecode128K == 0xC000)  // 0xFFFD: AY Register.
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

void Spectrum::updatePage()
{
    static size_t wrWait = 0;

    ++wrWait;
    if (wrWait == 5)
    {
        wrWait = 0;
        if ((paging & 0x20) == 0x00)
        {
            paging = z80.d;

            ramBank = paging & 0x07;
            romBank = ((paging & 0x10) == 0x00) ? 0 : 1;
            scrBank = ((paging & 0x08) == 0x00) ? 5 : 7;

            map[0] = &rom[romBank];
            map[3] = &ram[ramBank];

            contendedRam = ((paging & 0x01) == 0x01);   // RAM 1-3-5-7
            contendedRom = ((paging & 0x11) == 0x01);   // ROM 0, RAM 1-3-5-7
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
        ramBank = 0;
        romBank = 0;
        scrBank = 5;

        ula.maxPixel = 456;
        ula.maxScan = 311;
        
        paging = 0x00;
    }
    else
    {
        ramBank = 0;
        romBank = 0;
        scrBank = 5;

        ula.maxPixel = 448;
        ula.maxScan = 312;

        paging = 0x20;
    }

    map[0] = &rom[romBank];
    map[3] = &ram[ramBank];
}

// vim: et:sw=4:ts=4
