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
}

void Z80Decoder::startInstruction()
{
    regs.executionStep = 0;
}

uint_fast16_t Z80Decoder::getAddress()
{
    switch (regs.memAddrMode & 0x0F)
    {
        case 0x00:  // Direct Implicit:     LD A, B
            break;
        case 0x01:  // Direct Immediate:    LD A, n
            regs.addr.w = regs.pc.w;
            regs.pc.w++;
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
            regs.addr.w++;
            break;
        case 0x0A:  // Push                 PUSH AF
            regs.sp.w--;
            regs.addr.w = regs.sp.w;
            break;
        case 0x0B:  // Pop                  POP AF
            regs.addr.w = regs.sp.w;
            regs.sp.w++;
            break;
        default:
            break;
    }

    regs.memAddrMode >>= 4;
    
    return regs.addr.w;
}

void Z80Decoder::readByte(uint_fast8_t byte)
{
    regs.memRdCycles--;
    regs.iReg.l = regs.iReg.h;
    regs.iReg.h = byte;
}

uint_fast8_t Z80Decoder::writeByte()
{
    uint_fast8_t byte = regs.oReg.l;
    regs.oReg.l = regs.oReg.h;
    regs.memWrCycles--;
    return byte;
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
            finished = true;
            break;
        case PREFIX_DD:
            finished = (*DDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_DD | PREFIX_CB:
            finished = true;
        case PREFIX_FD:
            finished = (*FDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_FD | PREFIX_CB:
            finished = true;
            break;
        case PREFIX_NO:
            finished = (*unprefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        default:
            assert(false);
            break;
    }

    regs.executionStep++;
    return finished;
}

bool Z80Decoder::executeNmi()
{
    bool finished = z80Nmi(&regs);
    regs.executionStep++;
    return finished;
}

bool Z80Decoder::executeInt()
{
    bool finished = true;

    switch (regs.im)
    {
        case 0:
            finished = execute();
            break;

        case 1:
            decode(0xFF);   // RST 38h
            finished = execute();
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
