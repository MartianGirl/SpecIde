#include "Z80.h"

void Z80::reset()
{
    c &= ~SIGNAL_RESET_;
}

void Z80::clock()
{
    static uint_fast16_t c_d;
    static Z80State state = Z80State::ST_RESET;
    static bool nmiAccept = false;
    static bool nmiProcess = false;
    static bool intProcess = false;

    static uint_fast8_t iff = 0;

    // Process RESET signal
    if (!(c & SIGNAL_RESET_))
        state = Z80State::ST_RESET;

    // NMI is edge-triggered.
    // If a falling edge in NMI is detected, accept the NMI and clear
    // the IFF1 flag.
    if ((~c & c_d & SIGNAL_NMI_) == SIGNAL_NMI_)
    {
        nmiAccept = true;
        decoder.regs.iff &= ~(IFF1);
    }
    c_d = c;
    iff = decoder.regs.iff;

    // Process BUSRQ/BUSAK
    if ((c & SIGNAL_BUSRQ_) == SIGNAL_BUSRQ_)
        c |= SIGNAL_BUSAK_;

    switch (state)
    {
        case Z80State::ST_RESET:
            start();
            state = Z80State::ST_OCF_T1H_ADDRWR;
            return;

            // Machine cycle
        case Z80State::ST_OCF_T1H_ADDRWR:
            a = decoder.regs.pc.w;
            if (!intProcess && !nmiProcess && !(iff & HALT))
                ++decoder.regs.pc.w;
            c &= ~SIGNAL_M1_;
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_OCF_T1L_ADDRWR;
            return;

        case Z80State::ST_OCF_T1L_ADDRWR:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);
            state = Z80State::ST_OCF_T2H_DATARD;
            return;

        case Z80State::ST_OCF_T2H_DATARD:
            state = Z80State::ST_OCF_T2L_DATARD;
            return;

        case Z80State::ST_OCF_T2L_DATARD:
            if (((c & SIGNAL_WAIT_) == 0x0000) && !nmiProcess)
                state = Z80State::ST_OCF_T2H_DATARD;
            else
                state = Z80State::ST_OCF_T3H_RFSH1;
            return;

        case Z80State::ST_OCF_T3H_RFSH1:
            c |= (SIGNAL_MREQ_ | SIGNAL_IORQ_ | SIGNAL_RD_ | SIGNAL_M1_);
            a = decoder.regs.ir.w & 0xFF7F;
            if (!(iff & HALT))
                decoder.decode(d);
            decoder.startInstruction();
            decoder.regs.ir.l = (decoder.regs.ir.l & 0x80) 
                | ((decoder.regs.ir.l + 1) & 0x7F);
            state = Z80State::ST_OCF_T3L_RFSH1;
            return;

        case Z80State::ST_OCF_T3L_RFSH1:
            // c &= ~(SIGNAL_MREQ_); // | SIGNAL_RFSH_);
            state = Z80State::ST_OCF_T4H_RFSH2;
            return;

        case Z80State::ST_OCF_T4H_RFSH2:
            state = Z80State::ST_OCF_T4L_RFSH2;
            return;

        case Z80State::ST_OCF_T4L_RFSH2:
            // c |= (SIGNAL_MREQ_);
            break;

            // NMI. Machine cycle after NMI.
            // Maybe I'll merge these with ST_OCF_Tn, but for now I'll keep them
            // separate, since it is clearer.
            // I'll decide when I've implemented also the INT cycle.
        case Z80State::ST_NMI_T1H_ADDRWR:
            // This state could be done in ST_OCF_T1_ADDRWR by incrementing
            // PC only if NMI is false...
            a = decoder.regs.pc.w;
            // c |= SIGNAL_RFSH_;
            c &= ~SIGNAL_M1_;
            state = Z80State::ST_NMI_T1L_ADDRWR;
            return;

        case Z80State::ST_NMI_T1L_ADDRWR:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);
            state = Z80State::ST_NMI_T2H_DATARD;
            return;

        case Z80State::ST_NMI_T2H_DATARD:
            state = Z80State::ST_NMI_T2L_DATARD;
            return;

        case Z80State::ST_NMI_T2L_DATARD:
            state = Z80State::ST_OCF_T3H_RFSH1;
            return;

            // Maskable INT.
        case Z80State::ST_INT_T1H_ADDRWR:
            a = decoder.regs.pc.w;
            c &= ~SIGNAL_M1_;
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_INT_T1L_ADDRWR;
            return;

        case Z80State::ST_INT_T1L_ADDRWR:
            state = Z80State::ST_INT_T2H_DATARD;
            return;

        case Z80State::ST_INT_T2H_DATARD:
            state = Z80State::ST_INT_T2L_DATARD;
            return;

        case Z80State::ST_INT_T2L_DATARD:
            state = Z80State::ST_INT_T3H_WAIT1;
            return;

        case Z80State::ST_INT_T3H_WAIT1:
            state = Z80State::ST_INT_T3L_WAIT1;
            return;

        case Z80State::ST_INT_T3L_WAIT1:
            c &= ~SIGNAL_IORQ_;
            state = Z80State::ST_INT_T4H_WAIT2;
            return;

        case Z80State::ST_INT_T4H_WAIT2:
            state = Z80State::ST_INT_T4L_WAIT2;
            return;

        case Z80State::ST_INT_T4L_WAIT2:
            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_INT_T4H_WAIT2;
            else
                state = Z80State::ST_OCF_T3H_RFSH1;
            return;

            // Memory read cycle
        case Z80State::ST_MEMRD_T1H_ADDRWR:
            // INT mode 0 places bytes directly on the bus.
            // c |= SIGNAL_RFSH_;
            if (intProcess == false || decoder.regs.im == 2)
                a = decoder.getAddress();
            state = Z80State::ST_MEMRD_T1L_ADDRWR;
            return;

        case Z80State::ST_MEMRD_T1L_ADDRWR:
            c &= ~(SIGNAL_MREQ_ | SIGNAL_RD_);
            state = Z80State::ST_MEMRD_T2H_WAITST;
            return;

        case Z80State::ST_MEMRD_T2H_WAITST:
            state = Z80State::ST_MEMRD_T2L_WAITST;
            return;

        case Z80State::ST_MEMRD_T2L_WAITST:
            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_MEMRD_T2H_WAITST;
            else
                state = Z80State::ST_MEMRD_T3H_DATARD;
            return;

        case Z80State::ST_MEMRD_T3H_DATARD:
            state = Z80State::ST_MEMRD_T3L_DATARD;
            return;

        case Z80State::ST_MEMRD_T3L_DATARD:
            decoder.readMem(d);
            c |= (SIGNAL_MREQ_ | SIGNAL_RD_);
            break;

            // Memory write cycle
        case Z80State::ST_MEMWR_T1H_ADDRWR:
            a = decoder.getAddress();
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_MEMWR_T1L_ADDRWR;
            return;

        case Z80State::ST_MEMWR_T1L_ADDRWR:
            d = decoder.writeMem();
            c &= ~(SIGNAL_MREQ_);
            state = Z80State::ST_MEMWR_T2H_WAITST;
            return;

        case Z80State::ST_MEMWR_T2H_WAITST:
            state = Z80State::ST_MEMWR_T2L_WAITST;
            return;

        case Z80State::ST_MEMWR_T2L_WAITST:
            c &= ~(SIGNAL_WR_);

            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_MEMWR_T2H_WAITST;
            else
                state = Z80State::ST_MEMWR_T3H_DATAWR;
            return;

        case Z80State::ST_MEMWR_T3H_DATAWR:
            state = Z80State::ST_MEMWR_T3L_DATAWR;
            return;

        case Z80State::ST_MEMWR_T3L_DATAWR:
            c |= SIGNAL_MREQ_ | SIGNAL_WR_;
            break;

            // I/O read cycle
        case Z80State::ST_IORD_T1H_ADDRWR:
            // c |= SIGNAL_RFSH_;
            a = decoder.getAddress();
            state = Z80State::ST_IORD_T1L_ADDRWR;
            return;

        case Z80State::ST_IORD_T1L_ADDRWR:
            state = Z80State::ST_IORD_T2H_IORQ;
            return;

        case Z80State::ST_IORD_T2H_IORQ:
            c &= ~(SIGNAL_IORQ_ | SIGNAL_RD_);
            state = Z80State::ST_IORD_T2L_IORQ;
            return;

        case Z80State::ST_IORD_T2L_IORQ:
            state = Z80State::ST_IORD_TWH_WAITST;
            return;

        case Z80State::ST_IORD_TWH_WAITST:
            state = Z80State::ST_IORD_TWL_WAITST;
            return;

        case Z80State::ST_IORD_TWL_WAITST:
            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_IORD_TWH_WAITST;
            else
                state = Z80State::ST_IORD_T3H_DATARD;
            return;

        case Z80State::ST_IORD_T3H_DATARD:
            state = Z80State::ST_IORD_T3L_DATARD;
            return;

        case Z80State::ST_IORD_T3L_DATARD:
            decoder.readIo(d);
            c |= SIGNAL_IORQ_ | SIGNAL_RD_;
            break;

            // I/O write cycle
        case Z80State::ST_IOWR_T1H_ADDRWR:
            a = decoder.getAddress();
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_IOWR_T1L_ADDRWR;
            return;

        case Z80State::ST_IOWR_T1L_ADDRWR:
            d = decoder.writeIo();
            state = Z80State::ST_IOWR_T2H_IORQ;
            return;

        case Z80State::ST_IOWR_T2H_IORQ:
            c &= ~(SIGNAL_IORQ_ | SIGNAL_WR_);
            state = Z80State::ST_IOWR_T2L_IORQ;
            return;

        case Z80State::ST_IOWR_T2L_IORQ:
            state = Z80State::ST_IOWR_TWH_WAITST;
            return;

        case Z80State::ST_IOWR_TWH_WAITST:
            state = Z80State::ST_IOWR_TWL_WAITST;
            return;

        case Z80State::ST_IOWR_TWL_WAITST:
            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_IOWR_TWH_WAITST;
            else
                state = Z80State::ST_IOWR_T3H_DATAWR;
            return;

        case Z80State::ST_IOWR_T3H_DATAWR:
            state = Z80State::ST_IOWR_T3L_DATAWR;
            return;

        case Z80State::ST_IOWR_T3L_DATAWR:
            c |= SIGNAL_IORQ_ | SIGNAL_WR_;
            break;

            // CPU internal cycle. This is just a delimiter, it is almost
            // the same as ST_CPU_T0_WAITST, but it helps in setting end of cycle
            // points for BUSRQ/BUSAK, for instance...
            // The CPU internal cycle begins here, and it is extended by using
            // WAITSTates.
        case Z80State::ST_CPU_T0H_CPUPROC:
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_CPU_T0L_CPUPROC;
            return;

        case Z80State::ST_CPU_T0L_CPUPROC:
            decoder.cpuProcCycle();
            break;

            // Wait state
        case Z80State::ST_CPU_T0H_WAITST:
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_CPU_T0L_WAITST;
            return;

        case Z80State::ST_CPU_T0L_WAITST:
            break;

        default:
            assert(false);
            return;
    }

    // In case we've finished a CPU cycle...
    bool endOfCycle = false;

    // If we are in a BUSRQ cycle, we just insert wait states.
    if ((c & SIGNAL_BUSAK_) == SIGNAL_BUSAK_)
    {
        if (nmiProcess == true)
        {
            endOfCycle = decoder.executeNmi();
            if (endOfCycle 
                    && decoder.regs.prefix == PREFIX_NO
                    && decoder.regs.memRdCycles == 0
                    && decoder.regs.memWrCycles == 0
                    && decoder.regs.ioRdCycles == 0
                    && decoder.regs.ioWrCycles == 0
                    && decoder.regs.cpuProcCycles == 0)
                nmiProcess = false;
        }
        else if (intProcess == true)
        {
            endOfCycle = decoder.executeInt();
            if (endOfCycle 
                    && decoder.regs.prefix == PREFIX_NO
                    && decoder.regs.memRdCycles == 0
                    && decoder.regs.memWrCycles == 0
                    && decoder.regs.ioRdCycles == 0
                    && decoder.regs.ioWrCycles == 0
                    && decoder.regs.cpuProcCycles == 0)
                intProcess = false;
        }
        else
        {
            endOfCycle = decoder.execute();
        }
    }

    if (endOfCycle == false)    // Machine cycle not finished, insert a state.
        state = Z80State::ST_CPU_T0H_WAITST;
    // BUSRQ goes here, after a complete machine cycle.
    else if ((c & SIGNAL_BUSRQ_) == 0x0000)
    {
        c &= ~SIGNAL_BUSAK_;
        state = Z80State::ST_CPU_T0H_WAITST;
    }
    else if (decoder.regs.memRdCycles)      // Perform memory read cycles.
        state = Z80State::ST_MEMRD_T1H_ADDRWR;
    else if (decoder.regs.ioRdCycles)       // Perform I/O read cycles.
        state = Z80State::ST_IORD_T1H_ADDRWR;
    else if (decoder.regs.cpuProcCycles)    // Perform internal CPU cycles.
        state = Z80State::ST_CPU_T0H_CPUPROC;
    else if (decoder.regs.memWrCycles)      // Perform memory write cycles.
        state = Z80State::ST_MEMWR_T1H_ADDRWR;
    else if (decoder.regs.ioWrCycles)       // Perform I/O write cycles.
        state = Z80State::ST_IOWR_T1H_ADDRWR;
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
        c |= SIGNAL_HALT_;
        state = Z80State::ST_NMI_T1H_ADDRWR;
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
        c |= SIGNAL_HALT_;
        state = Z80State::ST_INT_T1H_ADDRWR;
    }
    else if ((decoder.regs.iff & HALT) == HALT)
    {
        c &= ~SIGNAL_HALT_;
        state = Z80State::ST_OCF_T1H_ADDRWR;
    }
    else
    {
        state = Z80State::ST_OCF_T1H_ADDRWR;
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
