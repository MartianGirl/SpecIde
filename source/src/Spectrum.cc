#include "Spectrum.h"

Spectrum::Spectrum() :
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
    ifstream ifs("48.rom", ifstream::binary);
    while (ifs.get(c))
        rom[0].memory[pos++] = c;
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
        map[1]->a = ula.a;
        map[1]->as_ = false;
        map[1]->rd_ = false;
        map[1]->wr_ = true;
        map[1]->clock();
        ula.d = map[1]->d;
    }
    else if (io_ == false && wr_ == false) // Is Z80 mastering and writing?
        ula.d = z80.d;

    ula.clock();
    z80.c = ula.c;

    // We clock the Z80 if the ULA allows.
    if (ula.cpuClock)
    {
        // Z80 gets data from the ULA or memory, only when reading.
        if (io_ == false && rd_ == false)
        {
            if ((z80.a & 0x0001) == 0x0000)
            {
                z80.d = ula.d;
            }
            // else if ((z80.a & 0x001F) == 0x001F)    // Kempston joystick.
            // z80.d = map[1]->d;
            else if (!ula.idle)
            {
                z80.d = map[1]->d;  // Get the byte from the video memory.
            }
            else
            {
                z80.d = 0xFF;
            }
        }
        else if (as_ == false)
        {
            // Bank 0: 0000h - ROM
            // Bank 1: 4000h - Contended memory
            // Bank 2: 8000h - Extended memory
            // Bank 3: C000h - Extended memory
            map[memArea]->a = z80.a;
            map[memArea]->d = z80.d;
            map[memArea]->as_ = as_;
            map[memArea]->rd_ = rd_;
            map[memArea]->wr_ = wr_;
            map[memArea]->clock();
            z80.d = map[memArea]->d;
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
