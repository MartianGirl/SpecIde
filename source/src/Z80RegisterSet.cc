#include "Z80RegisterSet.h"

Z80RegisterSet::Z80RegisterSet() :
    registerSet(0),
    af(&af_pair[registerSet]),
    bc(&bc_pair[registerSet]),
    de(&de_pair[registerSet]),
    hl(&hl_pair[registerSet]),
    r8{
        {&bc_pair[0].h, &bc_pair[0].l, &de_pair[0].h, &de_pair[0].l,
            &hl_pair[0].h, &hl_pair[0].l, nullptr, &af_pair[0].h},
        {&bc_pair[1].h, &bc_pair[1].l, &de_pair[1].h, &de_pair[1].l,
            &hl_pair[1].h, &hl_pair[1].l, nullptr, &af_pair[1].h}
    },
    rx8{
        {&bc_pair[0].h, &bc_pair[0].l, &de_pair[0].h, &de_pair[0].l,
            &ix.h, &ix.l, nullptr, &af_pair[0].h},
        {&bc_pair[1].h, &bc_pair[1].l, &de_pair[1].h, &de_pair[1].l,
            &ix.h, &ix.l, nullptr, &af_pair[1].h}
    },
    ry8{
        {&bc_pair[0].h, &bc_pair[0].l, &de_pair[0].h, &de_pair[0].l,
            &iy.h, &iy.l, nullptr, &af_pair[0].h},
        {&bc_pair[1].h, &bc_pair[1].l, &de_pair[1].h, &de_pair[1].l,
            &iy.h, &iy.l, nullptr, &af_pair[1].h}
    },
    rp{
        {&bc_pair[0].w, &de_pair[0].w, &hl_pair[0].w, &sp.w},
        {&bc_pair[1].w, &de_pair[1].w, &hl_pair[1].w, &sp.w}
    },
    rp2{
        {&bc_pair[0].w, &de_pair[0].w, &hl_pair[0].w, &af_pair[0].w},
        {&bc_pair[1].w, &de_pair[1].w, &hl_pair[1].w, &af_pair[1].w}
    },
    reg8(&r8[0][0]),
    regx8(&rx8[0][0]),
    regy8(&ry8[0][0]),
    regp(&rp[0][0]),
    regp2(&rp2[0][0]),
    x(0), y(0), z(0), p(0), q(0),
    prefix(0),
    executionStep(0),
    memRdCycles(0), memWrCycles(0)
{
}

void Z80RegisterSet::reset()
{
    // Clear PC
    pc.w = 0x0000;

    // Clear all registers
    af_pair[0].w = 0xFFFF; af_pair[1].w = 0xFFFF;
    bc_pair[0].w = 0xFFFF; bc_pair[1].w = 0xFFFF;
    de_pair[0].w = 0xFFFF; de_pair[1].w = 0xFFFF;
    hl_pair[0].w = 0xFFFF; hl_pair[1].w = 0xFFFF;

    ir.w = 0xFFFF;
    sp.w = 0xFFFF;
    ix.w = 0xFFFF;
    iy.w = 0xFFFF;

    iff = 0x00;
    im = 0x00;

    selectRegisterSet(0x00); // We've got to choose one, I guess?
}

void Z80RegisterSet::selectRegisterSet(size_t set)
{
    registerSet = set;
    af = &af_pair[registerSet];
    bc = &bc_pair[registerSet];
    de = &de_pair[registerSet];
    hl = &hl_pair[registerSet];

    reg8 = &r8[registerSet][0];
    regx8 = &rx8[registerSet][0];
    regy8 = &ry8[registerSet][0];
    regp = &rp[registerSet][0];
    regp2 = &rp2[registerSet][0];
}

// vim: et:sw=4:ts=4
