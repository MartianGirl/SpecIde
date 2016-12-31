#pragma once

/** Z80FDPrefixed.h
 *
 * Z80 instruction table for FD-prefixed opcodes.
 *
 */

#include "Z80Instruction.h"
#include "Z80Nop.h"
#include "Z80LdRegYRegY.h"

class Z80FDPrefixed
{
    public:
        // Instructions
        Z80Nop iNop; 
        Z80LdRegYRegY iLdRegYRegY;

        Z80Instruction* table[4][8][8];

        Z80FDPrefixed() :
            table {
                // x = 0
                {
                    // y = 0
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                },
                // x = 1
                {
                    // y = 0
                    {
                        &iLdRegYRegY,   // 01000000: LD B, B
                        &iLdRegYRegY,   // 01000001: LD B, C
                        &iLdRegYRegY,   // 01000010: LD B, D
                        &iLdRegYRegY,   // 01000011: LD B, E
                        &iLdRegYRegY,   // 01000100: LD B, IYh
                        &iLdRegYRegY,   // 01000101: LD B, IYl
                        &iNop,          // 01000110: LD B, (HL)
                        &iLdRegYRegY    // 01000111: LD B, A
                    },
                    // y = 1
                    {
                        &iLdRegYRegY,   // 01001000: LD C, B
                        &iLdRegYRegY,   // 01001001: LD C, C
                        &iLdRegYRegY,   // 01001010: LD C, D
                        &iLdRegYRegY,   // 01001011: LD C, E
                        &iLdRegYRegY,   // 01001100: LD C, IYh
                        &iLdRegYRegY,   // 01001101: LD C, IYl
                        &iNop,          // 01001110: LD C, (HL)
                        &iLdRegYRegY    // 01001111: LD C, A
                    },
                    // y = 2
                    {
                        &iLdRegYRegY,   // 01010000: LD D, B
                        &iLdRegYRegY,   // 01010001: LD D, C
                        &iLdRegYRegY,   // 01010010: LD D, D
                        &iLdRegYRegY,   // 01010011: LD D, E
                        &iLdRegYRegY,   // 01010100: LD D, IYh
                        &iLdRegYRegY,   // 01010101: LD D, IYl
                        &iNop,          // 01010110: LD D, (HL)
                        &iLdRegYRegY    // 01010111: LD D, A
                    },
                    // y = 3
                    {
                        &iLdRegYRegY,   // 01011000: LD E, B
                        &iLdRegYRegY,   // 01011001: LD E, C
                        &iLdRegYRegY,   // 01011010: LD E, D
                        &iLdRegYRegY,   // 01011011: LD E, E
                        &iLdRegYRegY,   // 01011100: LD E, IYh
                        &iLdRegYRegY,   // 01011101: LD E, IYl
                        &iNop,          // 01011110: LD E, (HL)
                        &iLdRegYRegY    // 01011111: LD E, A
                    },
                    // y = 4
                    {
                        &iLdRegYRegY,   // 01100000: LD H, B
                        &iLdRegYRegY,   // 01100001: LD H, C
                        &iLdRegYRegY,   // 01100010: LD H, D
                        &iLdRegYRegY,   // 01100011: LD H, E
                        &iLdRegYRegY,   // 01100100: LD H, IYh
                        &iLdRegYRegY,   // 01100101: LD H, IYl
                        &iNop,          // 01100110: LD H, (HL)
                        &iLdRegYRegY    // 01100111: LD H, A
                    },
                    // y = 5
                    {
                        &iLdRegYRegY,   // 01101000: LD L, B
                        &iLdRegYRegY,   // 01101001: LD L, C
                        &iLdRegYRegY,   // 01101010: LD L, D
                        &iLdRegYRegY,   // 01101011: LD L, E
                        &iLdRegYRegY,   // 01101100: LD L, IYh
                        &iLdRegYRegY,   // 01101101: LD L, IYl
                        &iNop,          // 01101110: LD L, (HL)
                        &iLdRegYRegY    // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iNop,          // 01110000: LD (HL), B
                        &iNop,          // 01110001: LD (HL), C
                        &iNop,          // 01110010: LD (HL), D
                        &iNop,          // 01110011: LD (HL), E
                        &iNop,          // 01110100: LD (HL), H
                        &iNop,          // 01110101: LD (HL), L
                        &iNop,          // 01110110: HALT
                        &iNop           // 01110111: LD (HL), A
                    },
                    // y = 7
                    {
                        &iLdRegYRegY,   // 01111000: LD A, B
                        &iLdRegYRegY,   // 01111001: LD A, C
                        &iLdRegYRegY,   // 01111010: LD A, D
                        &iLdRegYRegY,   // 01111011: LD A, E
                        &iLdRegYRegY,   // 01111100: LD A, IYh
                        &iLdRegYRegY,   // 01111101: LD A, IYl
                        &iNop,          // 01111110: LD A, (HL)
                        &iLdRegYRegY    // 01111111: LD A, A
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                },
                // x = 3
                {
                    // y = 0
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
