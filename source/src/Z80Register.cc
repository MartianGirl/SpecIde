#include "Z80Register.h"

Z80Register::Z80Register() :
    reg(0),
    pw(&reg),

#if SPECIDE_BYTE_ORDER == 1
    ph(reinterpret_cast<uint8_t*>(pw) + sizeof(uint_fast16_t) - 2),
    pl(reinterpret_cast<uint8_t*>(pw) + sizeof(uint_fast16_t) - 1),
#else
    ph(reinterpret_cast<uint8_t*>(pw) + 1),
    pl(reinterpret_cast<uint8_t*>(pw) + 0),
#endif 
    w(*pw),
    h(*ph),
    l(*pl)
{
}

Z80Register::~Z80Register()
{
}

void Z80Register::setw(uint_fast16_t value)
{
    reg = value;
    // *h = (value & 0x0000FF00) >> 8;
    // *l = (value & 0x000000FF);
}

uint_fast16_t Z80Register::getw()
{
    // return ((*h << 8) + *l);
    return reg;
}

// vim: et:sw=4:ts=4
