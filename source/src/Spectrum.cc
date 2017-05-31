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
    std::ifstream ifs("48.rom", std::ifstream::binary);
    while (ifs.get(c))
        rom[0].memory[pos++] = c;
}

void Spectrum::clock()
{
    // Clock the ULA. This generates video and contention signals.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;
    ula.clock();
    z80.c = ula.c;

    // If the ULA is reading, we interface with the memory.
    if (ula.hiz == false)
        ulaBusAccess();
    else if (ula.ulaRead == false)
        ula.d = z80.d;

    if (ula.cpuClock == true)
    {
        z80BusAccess();
        z80.clock();
    }
}

void Spectrum::ulaBusAccess()
{
    map[1]->a = ula.a;
    map[1]->rd_ = ula.rd_;
    map[1]->as_ = ula.as_;
    map[1]->clock();
    ula.d = map[1]->d;
}

void Spectrum::z80BusAccess()
{
    size_t memIndex = (z80.a & 0xC000) >> 14;
    map[memIndex]->a = z80.a & 0x3FFF;
    map[memIndex]->d = z80.d;
    map[memIndex]->as_ = (z80.c & SIGNAL_MREQ_) == SIGNAL_MREQ_;
    map[memIndex]->rd_ = (z80.c & SIGNAL_RD_) == SIGNAL_RD_;
    map[memIndex]->wr_ = (z80.c & SIGNAL_WR_) == SIGNAL_WR_;
    map[memIndex]->clock();
    if (ula.ulaRead)
        z80.d = ula.d;
    else
        z80.d = map[memIndex]->d;
}

void Spectrum::reset()
{
    z80.reset();
}

// vim: et:sw=4:ts=4
