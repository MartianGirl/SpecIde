#include "Memory.h"

Memory::Memory(size_t width, bool rom) :
    size(1 << width),
    mask(size - 1),
    addr(0),
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
    else
        d = 0xFF;
}

// vim: et:sw=4:ts=4
