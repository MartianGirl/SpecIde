#include "Z80Decoder.h"

Z80Decoder::Z80Decoder() :
    memRdCycles(0),
    memWrCycles(0),
    x(0),
    y(0),
    z(0),
    p(0),
    q(0)
{
}

void Z80Decoder::decode(uint_fast8_t opcode)
{
    x = (opcode & 0xC0) >> 6; // xx......
    y = (opcode & 0x38) >> 3; // ..yyy...
    z = (opcode & 0x07);      // .....zzz
    p = y >> 1;              // ..pp....
    q = y & 0x01;            // ....q...

    // LD r, n      - 00rrr110 nnnnnnnn
    // LD (HL), n   - 00110110 nnnnnnnn
    //           C7 - 11000111
    //           06 - 00000110
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
            switch (x)
            {
                case 0x00:
                    switch (z)
                    {
                        case 0x06:
                            {
                                if (y == 0x06)
                                {
                                    memRdCycles = 1;
                                    memWrCycles = 1;
                                    z80AddrMode = Z80AddressingMode::AM_INDIRECT_IMMEDIATE;
                                }
                                else
                                {
                                    memRdCycles = 1;
                                    memWrCycles = 0;
                                    z80AddrMode = Z80AddressingMode::AM_DIRECT_IMMEDIATE;
                                }
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case 0x01:
                    if (y == 0x06 && z == 0x06)
                    {
                        // HALT
                    }
                    else
                    {
                        // LD r, r'
                    }
                    break;

                case 0x02:
                    break;

                case 0x03:
                    break;

                default:
                    break;
                    // can't happen
            }
    }
}

void Z80Decoder::readByte(uint_fast8_t byte)
{
    memRdCycles--;
    operand.h = operand.l;
    operand.l = byte;
}

void Z80Decoder::execute()
{
    // LD r, n      - 00rrr110 nnnnnnnn
    // LD (HL), n   - 00110110 nnnnnnnn
    //           C7 - 11000111
    //           06 - 00000110
    switch (x)
    {
        case 0x00:
            switch (z)
            {
                case 0x06:
                    {
                        if (y == 0x06)
                        {
                            memWrCycles--;
                            wrAddress = regs.hl->w;
                        }
                        else
                        {
                            *(regs.reg8[y]) = operand.l;
                        }
                    }
                    break;

                default:
                    break;
            }
            break;

        case 0x01:
            if (y == 0x06)
            {
                if (z == 0x06)
                {
                    // HALT
                }
                else
                {
                    // LD (HL), r'
                }
            }
            else if (z == 0x06)
            {
                // LD r', (HL)
            }
            else
            {
                *(regs.reg8[y]) = *(regs.reg8[z]);
            }
            break;

        case 0x02:
            break;

        case 0x03:
            break;

        default:
            break;
            // can't happen
    }
}

void Z80Decoder::reset()
{
    regs.reset();
}

// vim: et:sw=4:ts=4
