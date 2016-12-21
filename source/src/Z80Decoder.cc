#include "Z80Decoder.h"

Z80Decoder::Z80Decoder() :
    registerSet(0),
    af(&af_pair[registerSet]),
    bc(&bc_pair[registerSet]),
    de(&de_pair[registerSet]),
    hl(&hl_pair[registerSet])
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
                        switch (y)
                        {
                            case 0x00: // LD B, n
                                bc->h = operand.l;
                                break;
                            case 0x01: // LD C, n
                                bc->l = operand.l;
                                break;
                            case 0x02: // LD D, n
                                de->h = operand.l;
                                break;
                            case 0x03: // LD E, n
                                de->l = operand.l;
                                break;
                            case 0x04: // LD H, n
                                hl->h = operand.l;
                                break;
                            case 0x05: // LD L, n
                                hl->l = operand.l;
                                break;
                            case 0x06: // LD (HL), n
                                memWrCycles--;
                                wrAddress = hl->w;
                                break;
                            case 0x07: // LD A, n
                                af->h = operand.l;
                                break;
                            default:
                                break;
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

void Z80Decoder::reset()
{
    // Clear all registers
    af_pair[0].w = 0x0000; af_pair[1].w = 0x0000;
    bc_pair[0].w = 0x0000; bc_pair[1].w = 0x0000;
    de_pair[0].w = 0x0000; de_pair[1].w = 0x0000;
    hl_pair[0].w = 0x0000; hl_pair[1].w = 0x0000;

    sp.w = 0x0000;
    ix.w = 0x0000;
    iy.w = 0x0000;

    selectRegisterSet(0x00); // We've got to choose one, I guess?
}

void Z80Decoder::selectRegisterSet(size_t set)
{
    registerSet = set;
    af = &af_pair[registerSet];
    bc = &bc_pair[registerSet];
    de = &de_pair[registerSet];
    hl = &hl_pair[registerSet];
}

// vim: et:sw=4:ts=4