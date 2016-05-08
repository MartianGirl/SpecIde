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
    // Clear all registers
    pc.w = 0x0000;

    af[0].w = 0x0000; af[1].w = 0x0000;
    bc[0].w = 0x0000; bc[1].w = 0x0000;
    de[0].w = 0x0000; de[1].w = 0x0000;
    hl[0].w = 0x0000; hl[1].w = 0x0000;

    ri.w = 0x0000;
    sp.w = 0x0000;
    ix.w = 0x0000;
    iy.w = 0x0000;

}

void Z80::clock()
{
    if (!reset_)
        state = ST_RESET;

    switch (state)
    {
        case ST_RESET:
            reset();
            break;

        default:
            break;
    }
}
// vim: et:sw=4:ts=4
