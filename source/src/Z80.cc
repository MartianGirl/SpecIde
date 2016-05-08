#include "Z80.h"

Z80::Z80() :
    CPU()
{
}

Z80::~Z80()
{
}

void Z80::reset()
{
    pc = 0;
}

void Z80::clock()
{
}
// vim: et:sw=4:ts=4
