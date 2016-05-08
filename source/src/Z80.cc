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
    c &= ~RESET_;
}

void Z80::clock()
{
    if (!(c & RESET_))
        state = ST_RESET;

    switch (state)
    {
        case ST_RESET:
            start();

            state = ST_M1_T0_ADDRWR;
            break;

        case ST_M1_T0_ADDRWR:
            a = pc.w;
            c = 0xFFFF & ~(MREQ_ | RD_ | M1_);

            if (!wait_)
                state = ST_M1_T1_DATARD;
            break;

        case ST_M1_T1_DATARD:
            a = pc.w;
            c = 0xFFFF & ~(MREQ_ | RD_ | M1_);

            state = ST_M1_T2_RFSH1;
            break;

            case ST_M1_T2_RFSH1;

        case ST_M1_T2_RFSH1:
            c = 0xFFFF & ~(MREQ_ | RFSH_);

            state = ST_M1_T3_RFSH2;
            break;

        case ST_M1_T3_RFSH2:
            c = 0xFFFF & ~(RFSH_);

        default:
            break;
    }
}

void Z80::start()
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

    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;
}

// vim: et:sw=4:ts=4
