#include "Memory.h"

Memory::Memory(std::size_t addrWidth, bool readOnly) :
    readOnly(readOnly)
{
    size = 1 << addrWidth;
    memory.assign(size, 0x00);
}

Memory::~Memory()
{
}

void Memory::clock()
{
    if (readOnly == false && write == true)
    {
        memory[addr % size] = data;
    }
    else
    {
        data = memory[addr % size];
    }
}

// vim: et:sw=4:ts=4
