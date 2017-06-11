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

    iff = decoder.regs.iff;

    if ((iff & HALT) == HALT)
        c &= ~SIGNAL_HALT_;
    else
        c |= SIGNAL_HALT_;
   
    if ((c & SIGNAL_BUSRQ_) == SIGNAL_BUSRQ_)
        c |= SIGNAL_BUSAK_;

    switch (state)
    {
        case Z80State::ST_RESET:
            start();
            state = Z80State::ST_OCF_T1_ADDRWR;
            break;

        // Machine cycle
        case Z80State::ST_OCF_T1_ADDRWR:
            a = decoder.regs.pc.w;
            if (!intProcess && !nmiProcess && !(iff & HALT))
                ++decoder.regs.pc.w;

            // c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            state = Z80State::ST_OCF_T2_DATARD;
            break;

        case Z80State::ST_OCF_T2_DATARD:
            c |= SIGNAL_MREQ_;
            c &= ~(SIGNAL_RD_ | SIGNAL_M1_);

            if (c & SIGNAL_WAIT_ && !nmiProcess)
                state = Z80State::ST_OCF_T3_RFSH1;
            break;

        case Z80State::ST_OCF_T3_RFSH1:
            a = decoder.regs.ir.w & 0xFF7F;
            if (!(iff & HALT))
                decoder.decode(d);
            decoder.startInstruction();
            decoder.regs.ir.l = (decoder.regs.ir.l & 0x80) 
                | ((decoder.regs.ir.l + 1) & 0x7F);

            c |= (SIGNAL_RD_ | SIGNAL_M1_ | SIGNAL_IORQ_ | SIGNAL_MREQ_);
            // c &= ~(SIGNAL_MREQ_ | SIGNAL_RFSH_);

            state = Z80State::ST_OCF_T4_RFSH2;
            break;

        case Z80State::ST_OCF_T4_RFSH2:
            // c |= (SIGNAL_MREQ_);
            // c &= ~(SIGNAL_RFSH_);

            state = finishMachineCycle();
            break;

        // NMI. Machine cycle after NMI.
        // Maybe I'll merge these with ST_OCF_Tn, but for now I'll keep them
        // separate, since it is clearer.
        // I'll decide when I've implemented also the INT cycle.
        case Z80State::ST_NMI_T1_ADDRWR:
            // This state could be done in ST_OCF_T1_ADDRWR by incrementing
            // PC only if NMI is false...
            a = decoder.regs.pc.w;

            // c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_ | SIGNAL_M1_);

            state = Z80State::ST_NMI_T2_DATARD;
            break;

        case Z80State::ST_NMI_T2_DATARD:
            c |= SIGNAL_MREQ_;
            c &= ~(SIGNAL_RD_ | SIGNAL_M1_);

            // This state could be done in ST_OCF_T2_DATARD by moving to refresh
            // if SIGNAL_WAIT_ is high, or if NMI...
            state = Z80State::ST_OCF_T3_RFSH1;
            break;

        case Z80State::ST_INT_T1_ADDRWR:
            a = decoder.regs.pc.w;

            // c |= SIGNAL_RFSH_;
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

            state = Z80State::ST_OCF_T3_RFSH1;
            break;

        // Memory read cycle
        case Z80State::ST_MEMRD_T1_ADDRWR:
            // INT mode 0 places bytes directly on the bus.
             if (intProcess == false || decoder.regs.im == 2)
                 a = decoder.getAddress();

            // c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);

            state = Z80State::ST_MEMRD_T2_WAITST;
            break;

        case Z80State::ST_MEMRD_T2_WAITST:
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_MEMRD_T3_DATARD;
            break;

        case Z80State::ST_MEMRD_T3_DATARD:
            decoder.readMem(d);

            c |= SIGNAL_MREQ_ | SIGNAL_RD_;

            state = finishMachineCycle();
            break;

        // Memory write cycle
        case Z80State::ST_MEMWR_T1_ADDRWR:
            a = decoder.getAddress();
            d = decoder.writeMem();

            // c |= SIGNAL_RFSH_;
            c &= ~(SIGNAL_MREQ_);

            state = Z80State::ST_MEMWR_T2_WAITST;
            break;

        case Z80State::ST_MEMWR_T2_WAITST:
            c &= ~(SIGNAL_WR_);

            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_MEMWR_T3_DATAWR;
            break;

        case Z80State::ST_MEMWR_T3_DATAWR:
            c |= SIGNAL_MREQ_ | SIGNAL_WR_;

            state = finishMachineCycle();
            break;

        // I/O read cycle
        case Z80State::ST_IORD_T1_ADDRWR:
            a = decoder.getAddress();

            // c |= SIGNAL_RFSH_;

            state = Z80State::ST_IORD_T2_IORQ;
            break;

        case Z80State::ST_IORD_T2_IORQ:
            c &= ~(SIGNAL_IORQ_ | SIGNAL_RD_);

            state = Z80State::ST_IORD_TW_WAITST;
            break;

        case Z80State::ST_IORD_TW_WAITST:
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_IORD_T3_DATARD;
            break;

        case Z80State::ST_IORD_T3_DATARD:
            decoder.readIo(d);

            c |= SIGNAL_IORQ_ | SIGNAL_RD_;
            state = finishMachineCycle();
            break;

            // I/O write cycle
        case Z80State::ST_IOWR_T1_ADDRWR:
            a = decoder.getAddress();
            d = decoder.writeIo();

            // c |= SIGNAL_RFSH_;

            state = Z80State::ST_IOWR_T2_IORQ;
            break;

        case Z80State::ST_IOWR_T2_IORQ:
            c &= ~(SIGNAL_IORQ_ | SIGNAL_WR_);

            state = Z80State::ST_IOWR_TW_WAITST;
            break;

        case Z80State::ST_IOWR_TW_WAITST:
            if (c & SIGNAL_WAIT_)
                state = Z80State::ST_IOWR_T3_DATAWR;
            break;

        case Z80State::ST_IOWR_T3_DATAWR:
            c |= SIGNAL_IORQ_ | SIGNAL_WR_;

            state = finishMachineCycle();
            break;

        // CPU internal cycle. This is just a delimiter, it is almost
        // the same as ST_CPU_T0_WAITST, but it helps in setting end of cycle
        // points for BUSRQ/BUSAK, for instance...
        // The CPU internal cycle begins here, and it is extended by using
        // WAITSTates.
        case Z80State::ST_CPU_T0_CPUPROC:
            // c |= SIGNAL_RFSH_;
            decoder.cpuProcCycle();

            state = finishMachineCycle();
            break;

        // Wait state
        case Z80State::ST_CPU_T0_WAITST:
            // c |= SIGNAL_RFSH_;

            state = finishMachineCycle();
            break;

        default:
            assert(false);
            break;
    }
}

Z80State Z80::finishMachineCycle()
{
    bool endOfCycle = false;

    // If we are in a BUSRQ cycle, we just insert wait states.
    if ((c & SIGNAL_BUSAK_) == SIGNAL_BUSAK_)
        endOfCycle = execute();
    
    if (endOfCycle == false)    // Machine cycle not finished, insert a state.
        return Z80State::ST_CPU_T0_WAITST;
    // BUSRQ goes here, after a complete machine cycle.
    else if ((c & SIGNAL_BUSRQ_) == 0x0000)
    {
        c &= ~SIGNAL_BUSAK_;
        return Z80State::ST_CPU_T0_WAITST;
    }
    else if (decoder.regs.memRdCycles)      // Perform memory read cycles.
        return Z80State::ST_MEMRD_T1_ADDRWR;
    else if (decoder.regs.ioRdCycles)       // Perform I/O read cycles.
        return Z80State::ST_IORD_T1_ADDRWR;
    else if (decoder.regs.cpuProcCycles)    // Perform internal CPU cycles.
        return Z80State::ST_CPU_T0_CPUPROC;
    else if (decoder.regs.memWrCycles)      // Perform memory write cycles.
        return Z80State::ST_MEMWR_T1_ADDRWR;
    else if (decoder.regs.ioWrCycles)       // Perform I/O write cycles.
        return Z80State::ST_IOWR_T1_ADDRWR;
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
        decoder.regs.iff &= ~HALT;
        return Z80State::ST_NMI_T1_ADDRWR;
    }
    // INT only happens if there is no NMI.
    else if (((c & SIGNAL_INT_) == 0x0000) 
            && ((iff & IFF1) == IFF1)
            && ((decoder.regs.iff & IFF1) == IFF1)
            && decoder.regs.prefix == PREFIX_NO)
    {
        decoder.regs.iff &= ~(IFF1 | IFF2);
        intProcess = true;
        decoder.regs.iff &= ~HALT;
        return Z80State::ST_INT_T1_ADDRWR;
    }
    else
        return Z80State::ST_OCF_T1_ADDRWR;
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
            && decoder.regs.memWrCycles == 0
            && decoder.regs.ioRdCycles == 0
            && decoder.regs.ioWrCycles == 0
            && decoder.regs.cpuProcCycles == 0);
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

    iff = 0x00;

    decoder.reset();
}

// vim: et:sw=4:ts=4
