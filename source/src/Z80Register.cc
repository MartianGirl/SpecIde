#include "Z80Register.h"

Z80Register::Z80Register()
{
}

Z80Register::~Z80Register()
{
}

void Z80Register::setw(uint_fast16_t w)
{
    h = (w & 0x0000FF00) >> 8;
    l = (w & 0x000000FF);
}

uint_fast16_t Z80Register::getw()
{
    return ((h << 8) + l);
}

// vim: et:sw=4:ts=4
