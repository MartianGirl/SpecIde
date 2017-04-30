#include "Z80Decoder.h"

Z80Decoder::Z80Decoder()
{
}

void Z80Decoder::decode(uint_fast8_t opcode)
{
    regs.x = (opcode & 0xC0) >> 6; // xx......
    regs.y = (opcode & 0x38) >> 3; // ..yyy...
    regs.z = (opcode & 0x07);      // .....zzz
    regs.p = regs.y >> 1;          // ..pp....
    regs.q = regs.y & 0x01;        // ....q...
    regs.opcode = opcode;
}

void Z80Decoder::startInstruction()
{
    regs.executionStep = 0;

    regs.memRdCycles = 0;
    regs.memWrCycles = 0;
    regs.ioRdCycles = 0;
    regs.ioWrCycles = 0;
    regs.cpuProcCycles = 0;
}

uint_fast16_t Z80Decoder::getAddress()
{
    switch (regs.memAddrMode & 0x0F)
    {
        case 0x00:  // Direct Implicit:     LD A, B
            break;
        case 0x01:  // Direct Immediate:    LD A, n
            regs.addr.w = regs.pc.w;
            ++regs.pc.w;
            break;
        case 0x02:  // Indirect HL:         LD A, (HL)
            regs.addr.w = regs.hl.w;
            break;
        case 0x03:  // Indirect BC:         LD A, (BC)
            regs.addr.w = regs.bc.w;
            break;
        case 0x04:  // Indirect DE:         LD A, (DE)
            regs.addr.w = regs.de.w;
            break;
        case 0x05:  // Indirect SP:
            break;
        case 0x06:  // Indexed IX+d, IY+d:  LD A, (IX+d), LD A, (IY+d)
        case 0x07:
            regs.addr.w = regs.tmp.w;
            break;
        case 0x08:  // Indirect nn:         LD A, (nn)
            regs.addr.w = regs.iReg.w;
            break;
        case 0x09:  // Indirect extended    LD HL, (nn) - high byte read
            ++regs.addr.w;
            break;
        case 0x0A:  // Push                 PUSH AF
            --regs.sp.w;
            regs.addr.w = regs.sp.w;
            break;
        case 0x0B:  // Pop                  POP AF
            regs.addr.w = regs.sp.w;
            ++regs.sp.w;
            break;
        case 0x0C:  // Interrupt Mode 2
            regs.addr.w = ((regs.ir.h << 8) | regs.opcode);
            break;
        case 0x0D:  // I/O Indirect n:      IN A, (n)
            regs.addr.w = (regs.af.h << 8) | regs.iReg.h;
            break;
        case 0x0E:  // I/O Indirect C:
            regs.addr.w = regs.bc.w;
            break;

        default:
            assert(false);
            break;
    }

    regs.memAddrMode >>= 4;
    
    return regs.addr.w;
}

void Z80Decoder::readMem(uint_fast8_t byte)
{
    regs.iReg.l = regs.iReg.h;
    regs.iReg.h = byte;
    --regs.memRdCycles;
}

uint_fast8_t Z80Decoder::writeMem()
{
    uint_fast8_t byte = regs.oReg.l;
    regs.oReg.l = regs.oReg.h;
    --regs.memWrCycles;
    return byte;
}

void Z80Decoder::readIo(uint_fast8_t byte)
{
    regs.iReg.l = regs.iReg.h;
    regs.iReg.h = byte;
    --regs.ioRdCycles;
}

uint_fast8_t Z80Decoder::writeIo()
{
    uint_fast8_t byte = regs.oReg.l;
    regs.oReg.l = regs.oReg.h;
    --regs.ioWrCycles;
    return byte;
}

void Z80Decoder::cpuProcCycle()
{
    --regs.cpuProcCycles;
}

bool Z80Decoder::execute()
{
    bool finished = true;

    switch (regs.prefix)
    {
        case PREFIX_ED:
            finished = (*EDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_CB:
            finished = (*CBPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_DD:
            finished = (*DDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_FD:
            finished = (*FDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_DD | PREFIX_CB:
        case PREFIX_FD | PREFIX_CB:
            finished = (*XXCBPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_NO:
            finished = (*unprefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        default:
            assert(false);
            break;
    }

    ++regs.executionStep;
    return finished;
}

bool Z80Decoder::executeNmi()
{
    bool finished = z80Nmi(&regs);
    ++regs.executionStep;
    return finished;
}

bool Z80Decoder::executeInt()
{
    bool finished = true;

    switch (regs.im)
    {
        case 0:             // Mode 0: Execute the instruction in the data bus.
            finished = execute();
            break;

        case 1:             // Mode 1: Execute RST 38h
            decode(0xFF);
            finished = execute();
            break;

        case 2:             // Mode 2: Jump to the address in IR.h & data bus.
            finished = z80IntMode2(&regs);
            ++regs.executionStep;
            break;

        default:
            assert(false);
            break;
    }

    return finished;
}

void Z80Decoder::reset()
{
    regs.reset();
}

// vim: et:sw=4:ts=4
