#include "Spectrum.h"
#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    joystick(0),
    kempston(false),
    spectrum128K(false),
    idle(0xFF),
    paging(0x20),
    ramBank(3), romBank(0), scrBank(5),
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
    map{&rom[0], &ram[5], &ram[2], &ram[3]}
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

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

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

            if (spectrum128K && ((z80.a & 0x8002) == 0x0000)       // Port 0x7FFD
                    && ((wr_ == false) || (spectrumPlus2 == false && rd_ == false)))
            {
                if ((paging & 0x20) == 0x00)
                {
                    paging &= 0x20; // Keep bit 5.
                    paging |= z80.d;

                    ramBank = paging & 0x07;
                    romBank = ((paging & 0x10) == 0x00) ? 0 : 1;
                    scrBank = ((paging & 0x08) == 0x00) ? 5 : 7;

                    map[0] = &rom[romBank];
                    map[3] = &ram[ramBank];

                    ula.memContentionMask = (ramBank & 0x01) ? 0x4000 : 0xC000;
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

void Spectrum::reset()
{
    ula.reset();    // Synchronize clock level.
    z80.reset();

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
        ramBank = 3;
        romBank = 0;
        scrBank = 5;

        ula.maxPixel = 448;
        ula.maxScan = 312;

        paging = 0x20;
    }

    map[0] = &rom[romBank];
    map[3] = &ram[ramBank];

    ula.memContentionMask = 0xC000;
}

// vim: et:sw=4:ts=4
