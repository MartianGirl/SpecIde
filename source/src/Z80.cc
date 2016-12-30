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
            a = decoder.regs.pc.w;
            decoder.regs.pc.w++;
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
            a = decoder.regs.ir.w & 0xFF7F;
            decoder.regs.ir.l = (decoder.regs.ir.l & 0x80) 
                | ((decoder.regs.ir.l + 1) & 0x7F);
            c |= (SIGNAL_RD_ | SIGNAL_M1_);
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);
            decoder.decode(d);

            state = Z80State::ST_M1_T4_RFSH2;
            break;

        case Z80State::ST_M1_T4_RFSH2:
            c |= (SIGNAL_MREQ_);
            c &= ~(SIGNAL_RFSH_);

            state = finishMemoryCycle();
            break;

        // M2. Memory read cycle
        case Z80State::ST_M2_T1_ADDRWR:

            // switch(decoder.z80AddrMode)
            // {
                // case Z80AddressingMode::AM_DIRECT_IMMEDIATE:
                // case Z80AddressingMode::AM_INDIRECT_IMMEDIATE:
                // case Z80AddressingMode::AM_INDEXED_IMMEDIATE:
                    // a = pc.w;
                    // pc.w++;
                    // break;
                    // 
                // default:
                    // Should not happen
                    // break;
            // }

            a = decoder.regs.address.w;
//            pc.w++;

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

            decoder.readByte(d);
            state = finishMemoryCycle();
            break;

        // M3. Memory write cycle
        case Z80State::ST_M3_T1_ADDRWR:

            // switch(decoder.z80AddrMode)
            // {
                // case Z80AddressingMode::AM_INDIRECT_IMMEDIATE:
                    // a = decoder.wrAddress;
                    // d = decoder.operand.l;
                    // break;

                // default:
                    // Should not happen
                    // break;
            // }

            a = decoder.regs.address.w;
            d = decoder.regs.operand.h;

            c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_);
            state = Z80State::ST_M3_T2_WAITST;
            break;

        case Z80State::ST_M3_T2_WAITST:
            c &= ~(SIGNAL_WR_);
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_M3_T3_DATAWR;
            break;

        case Z80State::ST_M3_T3_DATAWR:
            c |= SIGNAL_MREQ_ | SIGNAL_WR_;

            state = finishMemoryCycle();
            break;

        default:
            break;
    }
}

Z80State Z80::finishMemoryCycle()
{
    if (decoder.regs.memRdCycles)
        return Z80State::ST_M2_T1_ADDRWR;
    else if (decoder.regs.memWrCycles)
    {
        decoder.execute();
        return Z80State::ST_M3_T1_ADDRWR;
    }
    else
    {
        decoder.execute();
        return Z80State::ST_M1_T1_ADDRWR;
    }
}

void Z80::start()
{
    // Clear buses
    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;

    decoder.reset();
}

// vim: et:sw=4:ts=4
