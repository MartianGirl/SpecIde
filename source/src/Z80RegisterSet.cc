#include "Z80RegisterSet.h"

Z80RegisterSet::Z80RegisterSet() :
    reg8{&bc.h, &bc.l, &de.h, &de.l, &hl.h, &hl.l, nullptr, &af.h},
    regx8{&bc.h, &bc.l, &de.h, &de.l, &ix.h, &ix.l, nullptr, &af.h},
    regy8{&bc.h, &bc.l, &de.h, &de.l, &iy.h, &iy.l, nullptr, &af.h},
    regp{&bc.w, &de.w, &hl.w, &sp.w},
    regp2{&bc.w, &de.w, &hl.w, &af.w},
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
    af.w = 0xFFFF; af_.w = 0xFFFF;
    bc.w = 0xFFFF; bc_.w = 0xFFFF;
    de.w = 0xFFFF; de_.w = 0xFFFF;
    hl.w = 0xFFFF; hl_.w = 0xFFFF;

    ir.w = 0xFFFF;
    sp.w = 0xFFFF;
    ix.w = 0xFFFF;
    iy.w = 0xFFFF;

    iReg.w = 0xFFFF;
    oReg.w = 0xFFFF;
    addr.w = 0xFFFF;
    tmp.w = 0xFFFF;

    iff = 0x00;
    im = 0x00;
}

// vim: et:sw=4:ts=4
