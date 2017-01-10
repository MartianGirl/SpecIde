#include "Memory.h"

Memory::Memory(size_t width, bool rom) :
    size(1 << width),
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
        addr = a;

    if (rom == false && wr_ == false)
    {
        memory[addr % size] = d;
    }
    else if(rd_ == false)
    {
        d = memory[addr % size];
    }
}

// vim: et:sw=4:ts=4
