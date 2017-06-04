#include "Spectrum.h"

Spectrum::Spectrum() :
    as_(true), rd_(true), wr_(true),
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
    std::ifstream ifs("48.rom", std::ifstream::binary);
    while (ifs.get(c))
        rom[0].memory[pos++] = c;
}

void Spectrum::clock()
{
    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;
    // std::cout << "Pixel counter is: " << ula.pixel;
    // std::cout << " (" << (ula.pixel & 0x0F) << ")" << std::endl;
    ula.clock();
    // std::cout << "CPUwait is" << ula.cpuWait << std::endl;
    z80.c = ula.c;

    // Memory interface
    as_ = ((z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_);
    rd_ = ((z80.c & SIGNAL_RD_) == SIGNAL_RD_);
    wr_ = ((z80.c & SIGNAL_WR_) == SIGNAL_WR_);

    // Bank 0: 0000h - ROM
    map[0]->a = z80.a;
    map[0]->d = z80.d;
    map[0]->as_ = ((z80.a & 0xC000) != 0x0000) || as_;
    map[0]->rd_ = rd_;
    map[0]->wr_ = wr_;
    map[0]->clock();

    // Bank 1: 4000h - Contended memory
    map[1]->a = ula.hiz ? z80.a : ula.a;
    map[1]->d = z80.d;  // Only Z80 writes here.
    map[1]->as_ = ula.hiz && (((z80.a & 0xC000) != 0x4000) || as_);
    map[1]->rd_ = ula.hiz && rd_;
    map[1]->wr_ = ula.hiz ? wr_ : true;
    map[1]->clock();

    // Bank 2: 8000h - Extended memory
    map[2]->a = z80.a;
    map[2]->d = z80.d;
    map[2]->as_ = ((z80.a & 0xC000) != 0x8000) || as_;
    map[2]->rd_ = rd_;
    map[2]->wr_ = wr_;
    map[2]->clock();

    // Bank 3: C000h - Extended memory
    map[3]->a = z80.a;
    map[3]->d = z80.d;
    map[3]->as_ = ((z80.a & 0xC000) != 0xC000) || as_;
    map[3]->rd_ = rd_;
    map[3]->wr_ = wr_;
    map[3]->clock();

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    if (ula.hiz == false)           // Is ULA reading from memory?
        ula.d = map[1]->d;
    else if (ula.read == false)     // Is ULA not being read?
        ula.d = z80.d;

    // Z80 gets data from the ULA or memory.
    if (rd_ == false)
        z80.d = (ula.read == true) ? ula.d : map[(z80.a & 0xC000) >> 14]->d;

    if (ula.cpuClock == true)
    {
        //std::cout << "Z80 state is: " << static_cast<size_t>(z80.state) << std::endl;
        z80.clock();
    }
}

void Spectrum::reset()
{
    z80.reset();
}

// vim: et:sw=4:ts=4
