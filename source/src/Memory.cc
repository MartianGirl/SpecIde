/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

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
