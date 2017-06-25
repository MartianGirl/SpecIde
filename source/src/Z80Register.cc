#include "Z80Register.h"

// This works because the member variables were declared in this same order
// in the header file.
Z80Register::Z80Register() :
    reg(0x0000),
    pw(reinterpret_cast<uint16_t*>(&reg)),
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

void Z80Register::setw(uint_fast16_t value)
{
    reg = value;
}

uint_fast16_t Z80Register::getw()
{
    return reg;
}

// vim: et:sw=4:ts=4
