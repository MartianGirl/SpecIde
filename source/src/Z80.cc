#include "Z80.h"

void Z80::reset()
{
    c &= ~SIGNAL_RESET_;
}

void Z80::clock()
{
    // Process RESET signal
    if (!(c & SIGNAL_RESET_))
        state = Z80State::ST_RESET;

    // NMI is edge-triggered.
    // If a falling edge in NMI is detected, accept the NMI and clear
    // the IFF1 flag.
    if ((~c & c_d & SIGNAL_NMI_) == SIGNAL_NMI_)
    {
        nmiAccept = true;
        iff &= ~(IFF1);
    }

    // This at the end so that it is the "delayed" version.
    intAccept = ((c_d & SIGNAL_INT_) == 0x0000);

    c_d = c;
    iff_d = iff;

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
            a = pc.w;
            if (!intProcess && !nmiProcess && !(iff_d & HALT))
                ++pc.w;
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
            c |= (SIGNAL_IORQ_ | SIGNAL_RD_ | SIGNAL_M1_);
            a = ir.w;
            if (!(iff_d & HALT))
                decode(d);
            startInstruction();
            ir.l = (ir.l & 0x80) 
                | ((ir.l + 1) & 0x7F);
            state = Z80State::ST_OCF_T3L_RFSH1;
            return;

        case Z80State::ST_OCF_T3L_RFSH1:
            c &= ~(SIGNAL_MREQ_); // | SIGNAL_RFSH_);
            state = Z80State::ST_OCF_T4H_RFSH2;
            return;

        case Z80State::ST_OCF_T4H_RFSH2:
            state = Z80State::ST_OCF_T4L_RFSH2;
            return;

        case Z80State::ST_OCF_T4L_RFSH2:
            c |= (SIGNAL_MREQ_);
            break;

            // NMI. Machine cycle after NMI.
            // Maybe I'll merge these with ST_OCF_Tn, but for now I'll keep them
            // separate, since it is clearer.
            // I'll decide when I've implemented also the INT cycle.
        case Z80State::ST_NMI_T1H_ADDRWR:
            // This state could be done in ST_OCF_T1_ADDRWR by incrementing
            // PC only if NMI is false...
            a = pc.w;
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
            a = pc.w;
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
            if (intProcess == false || im == 2)
                a = getAddress();
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
            readMem(d);
            c |= (SIGNAL_MREQ_ | SIGNAL_RD_);
            break;

            // Memory write cycle
        case Z80State::ST_MEMWR_T1H_ADDRWR:
            a = getAddress();
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_MEMWR_T1L_ADDRWR;
            return;

        case Z80State::ST_MEMWR_T1L_ADDRWR:
            d = dout = writeMem();
            c &= ~(SIGNAL_MREQ_);
            state = Z80State::ST_MEMWR_T2H_WAITST;
            return;

        case Z80State::ST_MEMWR_T2H_WAITST:
            // d = dout; // These are unnecessary when MREQ goes low.
            state = Z80State::ST_MEMWR_T2L_WAITST;
            return;

        case Z80State::ST_MEMWR_T2L_WAITST:
            // d = dout;
            c &= ~(SIGNAL_WR_);

            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_MEMWR_T2H_WAITST;
            else
                state = Z80State::ST_MEMWR_T3H_DATAWR;
            return;

        case Z80State::ST_MEMWR_T3H_DATAWR:
            // d = dout;
            state = Z80State::ST_MEMWR_T3L_DATAWR;
            return;

        case Z80State::ST_MEMWR_T3L_DATAWR:
            // d = dout;
            c |= SIGNAL_MREQ_ | SIGNAL_WR_;
            break;

            // I/O read cycle
        case Z80State::ST_IORD_T1H_ADDRWR:
            // c |= SIGNAL_RFSH_;
            a = getAddress();
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
            readIo(d);
            c |= SIGNAL_IORQ_ | SIGNAL_RD_;
            break;

            // I/O write cycle
        case Z80State::ST_IOWR_T1H_ADDRWR:
            a = getAddress();
            // c |= SIGNAL_RFSH_;
            state = Z80State::ST_IOWR_T1L_ADDRWR;
            return;

        case Z80State::ST_IOWR_T1L_ADDRWR:
            d = dout = writeIo();
            state = Z80State::ST_IOWR_T2H_IORQ;
            return;

        case Z80State::ST_IOWR_T2H_IORQ:
            d = dout;
            c &= ~(SIGNAL_IORQ_ | SIGNAL_WR_);
            state = Z80State::ST_IOWR_T2L_IORQ;
            return;

        case Z80State::ST_IOWR_T2L_IORQ:
            // d = dout;    // These are unnecessary, since IORQ goes low.
            state = Z80State::ST_IOWR_TWH_WAITST;
            return;

        case Z80State::ST_IOWR_TWH_WAITST:
            // d = dout;
            state = Z80State::ST_IOWR_TWL_WAITST;
            return;

        case Z80State::ST_IOWR_TWL_WAITST:
            // d = dout;
            if ((c & SIGNAL_WAIT_) == 0x0000)
                state = Z80State::ST_IOWR_TWH_WAITST;
            else
                state = Z80State::ST_IOWR_T3H_DATAWR;
            return;

        case Z80State::ST_IOWR_T3H_DATAWR:
            // d = dout;
            state = Z80State::ST_IOWR_T3L_DATAWR;
            return;

        case Z80State::ST_IOWR_T3L_DATAWR:
            // d = dout;
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
            cpuProcCycle();
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
            endOfCycle = executeNmi();
            if (endOfCycle 
                    && prefix == PREFIX_NO
                    && memRdCycles == 0
                    && memWrCycles == 0
                    && ioRdCycles == 0
                    && ioWrCycles == 0
                    && cpuProcCycles == 0)
                nmiProcess = false;
        }
        else if (intProcess == true)
        {
            endOfCycle = executeInt();
            if (endOfCycle 
                    && prefix == PREFIX_NO
                    && memRdCycles == 0
                    && memWrCycles == 0
                    && ioRdCycles == 0
                    && ioWrCycles == 0
                    && cpuProcCycles == 0)
                intProcess = false;
        }
        else
        {
            endOfCycle = execute();
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
    else if (memRdCycles)      // Perform memory read cycles.
        state = Z80State::ST_MEMRD_T1H_ADDRWR;
    else if (ioRdCycles)       // Perform I/O read cycles.
        state = Z80State::ST_IORD_T1H_ADDRWR;
    else if (cpuProcCycles)    // Perform internal CPU cycles.
        state = Z80State::ST_CPU_T0H_CPUPROC;
    else if (memWrCycles)      // Perform memory write cycles.
        state = Z80State::ST_MEMWR_T1H_ADDRWR;
    else if (ioWrCycles)       // Perform I/O write cycles.
        state = Z80State::ST_IOWR_T1H_ADDRWR;
    // At this point we have completed a instruction. NMI and INT can
    // happen here.
    // NMI has priority over INT. Now we check the NMI latch, which is
    // set above, when a falling edge is detected in the NMI signal.
    // Since prefixes are handled as one-cycle instructions, we have
    // to check we're not in the middle of a instruction.
    else if (prefix == PREFIX_NO
            && nmiAccept == true)
    {
        nmiAccept = false;
        nmiProcess = true;
        iff &= ~HALT;
        c |= SIGNAL_HALT_;
        state = Z80State::ST_NMI_T1H_ADDRWR;
    }
    // INT only happens if there is no NMI.
    else if (intAccept == true
            && ((iff_d & IFF1) == IFF1)
            && ((iff & IFF1) == IFF1)
            && prefix == PREFIX_NO)
    {
        iff &= ~(IFF1 | IFF2);
        intAccept = false;
        intProcess = true;
        iff &= ~HALT;
        c |= SIGNAL_HALT_;
        state = Z80State::ST_INT_T1H_ADDRWR;
    }
    else if ((iff & HALT) == HALT)
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

    // Clear PC
    pc.w = 0x0000;

    // Clear all registers
    af.w = 0xFFFF; af_.w = 0xFFFF;
    bc.w = 0xFFFF; bc_.w = 0xFFFF;
    de.w = 0xFFFF; de_.w = 0xFFFF;
    hl.w = 0xFFFF; hl_.w = 0xFFFF;
    wz.w = 0xFFFF; wz_.w = 0xFFFF;

    ir.w = 0x0000;
    sp.w = 0xFFFF;
    ix.w = 0xFFFF;
    iy.w = 0xFFFF;

    iReg.w = 0xFFFF;
    oReg.w = 0xFFFF;
    addr.w = 0xFFFF;
    acc.w = 0xFFFF;
    tmp.w = 0xFFFF;
    flg.w = 0xFFFF;

    iff = 0x00;
    im = 0x00;
}

void Z80::decode(uint_fast8_t byte)
{
    opcode = byte;
    x = (opcode & 0xC0) >> 6;   // xx......
    y = (opcode & 0x38) >> 3;   // ..yyy...
    z = (opcode & 0x07);        // .....zzz
    p = y >> 1;                 // ..pp....
    q = y & 0x01;               // ....q...
}

void Z80::startInstruction()
{
    executionStep = 0;

    memRdCycles = 0;
    memWrCycles = 0;
    ioRdCycles = 0;
    ioWrCycles = 0;
    cpuProcCycles = 0;
}

uint_fast16_t Z80::getAddress()
{
    switch (memAddrMode & 0x0F)
    {
        case 0x00:  // Direct Implicit:     LD A, B
            break;
        case 0x01:  // Direct Immediate:    LD A, n
            addr.w = pc.w;
            ++pc.w;
            break;
        case 0x02:  // Indirect HL:         LD A, (HL)
            addr.w = hl.w;
            break;
        case 0x03:  // Indirect BC:         LD A, (BC)
            addr.w = bc.w;
            break;
        case 0x04:  // Indirect DE:         LD A, (DE)
            addr.w = de.w;
            break;
        case 0x08:  // Indirect nn:         LD A, (nn)
        case 0x0C:  // Interrupt Mode 2
        case 0x0D:  // I/O Indirect n:      IN A, (n)
        case 0x0E:  // I/O Indirect C:
            addr.w = wz.w;
            ++wz.w;
            break;
        case 0x05:  // Indirect SP:
            break;
        case 0x06:  // Indexed IX+d, IY+d:  LD A, (IX+d), LD A, (IY+d)
        case 0x07:
        case 0x09:  // Indirect extended    LD HL, (nn) - high byte read
            addr.w = wz.w;
            break;
        case 0x0A:  // Push                 PUSH AF
            --sp.w;
            addr.w = sp.w;
            break;
        case 0x0B:  // Pop                  POP AF
            addr.w = sp.w;
            ++sp.w;
            break;

        default:
            assert(false);
            break;
    }

    memAddrMode >>= 4;
    
    return addr.w;
}

void Z80::readMem(uint_fast8_t byte)
{
    iReg.l = iReg.h;
    iReg.h = byte;
    --memRdCycles;
}

uint_fast8_t Z80::writeMem()
{
    uint_fast8_t byte = oReg.l;
    oReg.l = oReg.h;
    --memWrCycles;
    return byte;
}

void Z80::readIo(uint_fast8_t byte)
{
    iReg.l = iReg.h;
    iReg.h = byte;
    --ioRdCycles;
}

uint_fast8_t Z80::writeIo()
{
    uint_fast8_t byte = oReg.l;
    oReg.l = oReg.h;
    --ioWrCycles;
    return byte;
}

void Z80::cpuProcCycle()
{
    --cpuProcCycles;
}

bool Z80::execute()
{
    bool finished = true;

    switch (prefix)
    {
        case PREFIX_ED:
            finished = (this->*(edprefixed[opcode]))();
            break;
        case PREFIX_CB:
            finished = (this->*(cbprefixed[opcode]))();
            break;
        case PREFIX_DD:
            finished = (this->*(ddprefixed[opcode]))();
            break;
        case PREFIX_FD:
            finished = (this->*(fdprefixed[opcode]))();
            break;
        case PREFIX_DD | PREFIX_CB:
        case PREFIX_FD | PREFIX_CB:
            finished = (this->*(xxcbprefixed[opcode]))();
            break;
        case PREFIX_NO:
            finished = (this->*(unprefixed[opcode]))();
            break;
        default:
            assert(false);
            break;
    }

    ++executionStep;
    return finished;
}

bool Z80::executeNmi()
{
    bool finished = z80Nmi();
    ++executionStep;
    return finished;
}

bool Z80::executeInt()
{
    bool finished = true;

    switch (im)
    {
        case 0:             // Mode 0: Execute the instruction in the data bus.
            finished = execute();
            break;

        case 1:             // Mode 1: Execute RST 38h
            decode(0xFF);
            finished = execute();
            break;

        case 2:             // Mode 2: Jump to the address in IR.h & data bus.
            finished = z80IntMode2();
            ++executionStep;
            break;

        default:
            assert(false);
            break;
    }

    return finished;
}

// vim: et:sw=4:ts=4
