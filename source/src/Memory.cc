#include "Memory.h"

Memory::Memory(size_t width, bool rom) :
    size(1 << width),
    mask(size - 1),
    memory(size, 0x00),
    rom(rom),
    wr_(true),
    rd_(true),
    as_(true),
    a(0),
    d(0)
{
}

void Memory::clock()
{
    // Latch address bus
    if (as_ == false)
    {
        if (rom == false && wr_ == false)
            memory[a & mask] = d;
        else if(rd_ == false)
            d = memory[a & mask];
    }
}

uint_fast8_t Memory::read(uint_fast16_t addr)
{
    d = memory[addr & mask];
    return d;
}

void Memory::write(uint_fast16_t addr, uint_fast8_t data)
{
    if (rom == false)
        memory[addr & mask] = data;
}

// vim: et:sw=4:ts=4
