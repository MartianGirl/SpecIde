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

void Z80::updateNmi()
{
    // NMI is edge-triggered.
    if (!(c & SIGNAL_NMI_))
    {
        // If falling edge in NMI is detected, accept the NMI and clear
        // the IFF1 flag.
        if (nmiDelayed == false)
        {
            nmiAccept = true;
            decoder.regs.iff &= ~(IFF1);
        }
        nmiDelayed = true;
    }
    else
    {
        nmiDelayed = false;
    }
}

void Z80::clock()
{
    if (!(c & SIGNAL_RESET_))
        state = Z80State::ST_RESET;

    updateNmi();

    switch (state)
    {
        case Z80State::ST_RESET:
            start();

            state = Z80State::ST_M1_T1_ADDRWR;
            break;

        // M1. Machine cycle
        case Z80State::ST_M1_T1_ADDRWR:
            a = decoder.regs.pc.w;

            if (!intProcess && !nmiProcess)
                decoder.regs.pc.w++;

            c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            state = Z80State::ST_M1_T2_DATARD;
            break;

        case Z80State::ST_M1_T2_DATARD:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            if (c & SIGNAL_WAIT_ && !nmiProcess)
                state = Z80State::ST_M1_T3_RFSH1;
            break;

        case Z80State::ST_M1_T3_RFSH1:
            a = decoder.regs.ir.w & 0xFF7F;
            decoder.decode(d);
            decoder.startInstruction();
            decoder.regs.ir.l = (decoder.regs.ir.l & 0x80) 
                | ((decoder.regs.ir.l + 1) & 0x7F);
            c |= (SIGNAL_RD_ | SIGNAL_M1_ | SIGNAL_IORQ_);
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);

            state = Z80State::ST_M1_T4_RFSH2;
            break;

        case Z80State::ST_M1_T4_RFSH2:
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
            state = Z80State::ST_M1_T3_RFSH1;
            break;

        case Z80State::ST_INT_T1_ADDRWR:
            a = decoder.regs.pc.w;
            c |= SIGNAL_RFSH_;
            c &= ~SIGNAL_M1_;

            state = Z80State::ST_INT_T2_DATARD;
            break;

        case Z80State::ST_INT_T2_DATARD:
            c &= ~SIGNAL_M1_;

            state = Z80State::ST_INT_T3_WAIT1;
            break;

        case Z80State::ST_INT_T3_WAIT1:
            c &= ~(SIGNAL_M1_ | SIGNAL_IORQ_);

            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_INT_T4_WAIT2;
            break;

        case Z80State::ST_INT_T4_WAIT2:
            c &= ~(SIGNAL_M1_ | SIGNAL_IORQ_);

            state = Z80State::ST_M1_T3_RFSH1;
            break;

        // M2. Memory read cycle
        case Z80State::ST_M2_T1_ADDRWR:
            if (!intProcess)
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
    bool endOfCycle = execute();
    
    if (endOfCycle == false)    // Extend machine cycle. Some instructions do.
        return Z80State::ST_M0_T0_WAITST;
    // BUSRQ goes here, after a complete machine cycle.
    // I'd have to consider if these extra states are actually part of the
    // machine cycle, or if they are something else.
    else if (decoder.regs.memRdCycles)  // Perform read cycles.
        return Z80State::ST_M2_T1_ADDRWR;
    else if (decoder.regs.memWrCycles)  // Perform write cycles.
        return Z80State::ST_M3_T1_ADDRWR;
    // At this point we have completed a instruction. NMI and INT can
    // happen here.
    // NMI has priority over INT. Now we check the NMI latch, which is
    // set above, when a falling edge is detected in the NMI signal.
    // Since prefixes are handled as one-cycle instructions, we have
    // to check we're not in the middle of a instruction.
    else if (decoder.regs.prefix == PREFIX_NO
            && nmiAccept == true)
    {
        nmiAccept = false;
        nmiProcess = true;
        return Z80State::ST_NMI_T1_ADDRWR;
    }
    // INT only happens if there is no NMI.
    else if (((c & SIGNAL_INT_) == 0x0000) 
            && ((decoder.regs.iff & IFF1) == IFF1))
    {
        decoder.regs.iff &= ~(IFF1 | IFF2);
        intProcess = true;
        return Z80State::ST_INT_T1_ADDRWR;
    }
    else
        return Z80State::ST_M1_T1_ADDRWR;
}

bool Z80::execute()
{
    bool endOfCycle = true;

    if (nmiProcess == true)
    {
        endOfCycle = decoder.executeNmi();
        if (endOfCycle && endOfInstruction())
            nmiProcess = false;
    }
    else if (intProcess == true)
    {
        endOfCycle = decoder.executeInt();
        if (endOfCycle && endOfInstruction())
            intProcess = false;
    }
    else
    {
        endOfCycle = decoder.execute();
    }

    return endOfCycle;
}

bool Z80::endOfInstruction()
{
    return (decoder.regs.prefix == PREFIX_NO
            && decoder.regs.memRdCycles == 0
            && decoder.regs.memWrCycles == 0);
}

void Z80::start()
{
    // Clear buses
    a = 0xFFFF;
    c = 0xFFFF;
    d = 0xFF;

    nmiAccept = false;
    nmiDelayed = false;
    nmiProcess = false;

    intProcess = false;

    decoder.reset();
}

// vim: et:sw=4:ts=4
