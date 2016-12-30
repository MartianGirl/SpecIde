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

    switch (opcode)
    {
        case 0xCB:
            prefix |= PREFIX_CB;
            break;

        case 0xDD:
            prefix |= PREFIX_DD;
            break;

        case 0xED:
            prefix |= PREFIX_ED;
            break;

        case 0xFD:
            prefix |= PREFIX_FD;
            break;

        default:
            break;
    }

    unprefixed.table[regs.x][regs.y][regs.z]->decode(&regs);
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
    (*unprefixed.table[regs.x][regs.y][regs.z])(&regs);
}

void Z80Decoder::reset()
{
    regs.reset();
}

// vim: et:sw=4:ts=4
