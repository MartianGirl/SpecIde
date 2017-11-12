#include "Spectrum.h"
#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    joystick(0),
    kempston(false),
    idle(0xFF),
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
    map{&rom[0], &ram[0], &ram[1], &ram[2]}
{
    size_t pos = 0;
    char c;

    ifstream ifs;
    string romPath("/usr/share/spectrum-roms/");
    string romName("48.rom");

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
        rom[0].memory[pos++] = c;

    ifs.close();

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
        // Bank 1: 4000h - Contended memory
        ula.d = map[1]->read(ula.a);
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
                    z80.d = map[1]->d & idle;  // Get the byte from the video memory.
                }
                else
                {
                    z80.d = idle;
                }
            }
        }
        else if (as_ == false)
        {
            // Bank 0: 0000h - ROM
            // Bank 1: 4000h - Contended memory
            // Bank 2: 8000h - Extended memory
            // Bank 3: C000h - Extended memory
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
}

// vim: et:sw=4:ts=4
