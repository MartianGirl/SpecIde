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

    switch (regs.prefix)
    {
        case PREFIX_ED:
            EDPrefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
        case PREFIX_CB:
            break;
        case PREFIX_DD:
        case PREFIX_DD | PREFIX_CB:
            break;
        case PREFIX_FD:
        case PREFIX_FD | PREFIX_CB:
            break;
        default:
            unprefixed.table[regs.x][regs.y][regs.z]->decode(&regs); break;
    }
}

void Z80Decoder::readByte(uint_fast8_t byte)
{
    regs.memRdCycles--;
    regs.operand.l = regs.operand.h;
    regs.operand.h = byte;
}

void Z80Decoder::writeByte(uint_fast16_t addr)
{
    regs.memWrCycles--;
    regs.address.w = addr;
}

void Z80Decoder::execute()
{
    switch (regs.prefix)
    {
        case PREFIX_ED:
            (*EDPrefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
        case PREFIX_CB:
            break;
        case PREFIX_DD:
        case PREFIX_DD | PREFIX_CB:
            break;
        case PREFIX_FD:
        case PREFIX_FD | PREFIX_CB:
            break;
        default:
            (*unprefixed.table[regs.x][regs.y][regs.z])(&regs);
            break;
    }
}

void Z80Decoder::waitState()
{
    regs.cpuWtCycles--;
}

void Z80Decoder::reset()
{
    regs.reset();
}

// vim: et:sw=4:ts=4
