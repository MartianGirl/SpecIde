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

    regs.executionStep = 0;

/*    switch (regs.prefix)
    {
        case PREFIX_ED:
            EDPrefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
        case PREFIX_CB:
            break;
        case PREFIX_DD:
            DDPrefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
        case PREFIX_DD | PREFIX_CB:
            break;
        case PREFIX_FD:
            FDPrefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
        case PREFIX_FD | PREFIX_CB:
            break;
        default:
            unprefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
    } */
}

uint_fast16_t Z80Decoder::getAddress()
{
    switch (regs.memAddrMode & 0x0F)
    {
        case 0x00:  // Direct Implicit:     LD A, B
            break;
        case 0x01:  // Direct Immediate:    LD A, n
            regs.address.w = regs.pc.w;
            regs.pc.w++;
            break;
        case 0x02:  // Indirect HL:         LD A, (HL)
            regs.address.w = regs.hl->w;
            break;
        default:
            break;
    }

    regs.memAddrMode >>= 4;
    
    return regs.address.w;
}

void Z80Decoder::readByte(uint_fast8_t byte)
{
    regs.memRdCycles--;
    regs.operand.l = regs.operand.h;
    regs.operand.h = byte;
}

void Z80Decoder::writeByte()
{
    regs.memWrCycles--;
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
        default:
            finished = (*unprefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
    }

    regs.executionStep++;
    return finished;
}

void Z80Decoder::reset()
{
    regs.reset();
}

// vim: et:sw=4:ts=4
