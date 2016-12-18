#include "Z80.h"

Z80::Z80() :
    CPU(),
    state(Z80State::ST_RESET)
{
}

Z80::~Z80()
{
}

void Z80::reset()
{
    c &= ~SIGNAL_RESET_;
}

void Z80::clock()
{
    if (!(c & SIGNAL_RESET_))
        state = Z80State::ST_RESET;

    switch (state)
    {
        // M1. Machine cycle
        case Z80State::ST_RESET:
            start();

            state = Z80State::ST_M1_T1_ADDRWR;
            break;

        case Z80State::ST_M1_T1_ADDRWR:
            a = pc.w;
            pc.w++;
            c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);
            state = Z80State::ST_M1_T2_DATARD;
            break;

        case Z80State::ST_M1_T2_DATARD:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_M1_T3_RFSH1;
            break;

        case Z80State::ST_M1_T3_RFSH1:
            a = ir.w & 0xFF7F;
            c |= (SIGNAL_RD_ | SIGNAL_M1_);
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);
            // Opcode is sampled HERE
            opcode = d;

            state = Z80State::ST_M1_T4_RFSH2;
            break;

        case Z80State::ST_M1_T4_RFSH2:
            c |= (SIGNAL_MREQ_);
            c &= ~(SIGNAL_RFSH_);

            // LD r,n
            if ((opcode & 0xC7) == 0x06)
            {
                operand1 = (opcode & 0x38) >> 3;
                operand2 = 0x08;
                address = pc.w;
                pc.w++;
                state = Z80State::ST_M2_T1_ADDRWR;
            }
            break;

        case Z80State::ST_M2_T1_ADDRWR:
            a = address;
            c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);
            state = Z80State::ST_M2_T2_WAITST;
            break;

        case Z80State::ST_M2_T2_WAITST:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_M2_T3_DATARD;
            break;

        case Z80State::ST_M2_T3_DATARD:
            c |= SIGNAL_MREQ_ | SIGNAL_RD_;

            if ((opcode & 0xC7) == 0x06)
            {
                switch (operand1)
                {
                    case 0x00: 
                        bc[registerSet].h = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD B, n
                    case 0x01:
                        bc[registerSet].l = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD C, n
                    case 0x02:
                        de[registerSet].h = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD D, n
                    case 0x03:
                        de[registerSet].l = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD E, n
                    case 0x04:
                        hl[registerSet].h = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD H, n
                    case 0x05:
                        hl[registerSet].l = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD L, n
                    case 0x06:
                        indirect_addr = d;   // LD A, (nn)
                        break;
                    case 0x07:
                        af[registerSet].h = d;
                        state = Z80State::ST_M1_T1_ADDRWR;
                        break; // LD A, n
                    default: break;
                }
            }
            break;

        case Z80State::ST_M3_T1_ADDRWR:
            break;

        case Z80State::ST_M3_T2_WAITST:
            break;

        case Z80State::ST_M3_T3_DATAWR:
            break;



        default:
            break;
    }
}

void Z80::start()
{
    // Clear all registers
    pc.w = 0x0000;

    af[0].w = 0x0000; af[1].w = 0x0000;
    bc[0].w = 0x0000; bc[1].w = 0x0000;
    de[0].w = 0x0000; de[1].w = 0x0000;
    hl[0].w = 0x0000; hl[1].w = 0x0000;

    ir.w = 0x0000;
    sp.w = 0x0000;
    ix.w = 0x0000;
    iy.w = 0x0000;

    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;

    registerSet = 0x00; // We've got to choose one, I guess?
}

// vim: et:sw=4:ts=4
