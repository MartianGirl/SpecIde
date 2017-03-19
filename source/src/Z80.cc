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

    // NMI is edge-triggered.
    if (!(c & SIGNAL_NMI_))
    {
        if (nmiDelayed == false)
            nmiRequest = true;

        nmiDelayed = true;
    }
    else
    {
        nmiDelayed = false;
    }

    switch (state)
    {
        case Z80State::ST_RESET:
            start();

            state = Z80State::ST_M1_T1_ADDRWR;
            break;

        // M1. Machine cycle
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
        case Z80State::ST_NMI_T3_RFSH1:
            a = decoder.regs.ir.w & 0xFF7F;
            decoder.decode(d);
            decoder.regs.ir.l = (decoder.regs.ir.l & 0x80) 
                | ((decoder.regs.ir.l + 1) & 0x7F);
            c |= (SIGNAL_RD_ | SIGNAL_M1_);
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);

            state = Z80State::ST_M1_T4_RFSH2;
            break;

        case Z80State::ST_M1_T4_RFSH2:
        case Z80State::ST_NMI_T4_RFSH2:
            c |= (SIGNAL_MREQ_);
            c &= ~(SIGNAL_RFSH_);

            state = finishMemoryCycle();
            break;

        // NMI. Machine cycle after NMI.
        // Maybe I'll merge these with ST_M1_Tn, but for now I'll keep them
        // separate, since it is clearer.
        // I'll decide when I've implemented also the INT cycle.
        case Z80State::ST_NMI_T1_ADDRWR:
            // This state could be done in ST_M1_T1_ADDRWR by incrementing
            // PC only if NMI is false...
            a = decoder.regs.pc.w;
            c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            state = Z80State::ST_NMI_T2_DATARD;
            break;

        case Z80State::ST_NMI_T2_DATARD:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            // This state could be done in ST_M1_T2_DATARD by moving to refresh
            // if SIGNAL_WAIT_ is high, or if NMI...
            state = Z80State::ST_NMI_T3_RFSH1;
            break;

        // M2. Memory read cycle
        case Z80State::ST_M2_T1_ADDRWR:
            a = decoder.getAddress();
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
            decoder.readByte(d);
            c |= SIGNAL_MREQ_ | SIGNAL_RD_;

            state = finishMemoryCycle();
            break;

        // M3. Memory write cycle
        case Z80State::ST_M3_T1_ADDRWR:
            a = decoder.getAddress();
            d = decoder.writeByte();

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

        // M0. Wait state / Execution cycle (?)
        case Z80State::ST_M0_T0_WAITST:
            c |= SIGNAL_RFSH_;
            state = finishMemoryCycle();
            break;

        default:
            assert(false);
            break;
    }
}

Z80State Z80::finishMemoryCycle()
{
    bool finished = true;

    if (nmiProcess == true)
        finished = decoder.executeNmi();
    else
        finished = decoder.execute();
    
    if (finished == false)
        return Z80State::ST_M0_T0_WAITST;
    else if (decoder.regs.memRdCycles)
        return Z80State::ST_M2_T1_ADDRWR;
    else if (decoder.regs.memWrCycles)
        return Z80State::ST_M3_T1_ADDRWR;
    else if (nmiRequest == true && (decoder.regs.prefix == PREFIX_NO))
    {
        nmiRequest = false;
        nmiProcess = true;
        return Z80State::ST_NMI_T1_ADDRWR;
    }
    // else if (c & ~(SIGNAL_INT_) && decoder.regs.iff1
    // return Z80State::ST_INT_T1_ADDRWR;
    else
    {
        nmiProcess = false;
        return Z80State::ST_M1_T1_ADDRWR;
    }
}

void Z80::start()
{
    // Clear buses
    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;

    nmiDelayed = false;
    nmiProcess = false;
    nmiRequest = false;

    decoder.reset();
}

// vim: et:sw=4:ts=4
