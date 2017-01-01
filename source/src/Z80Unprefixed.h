#pragma once

/** Z80Unprefixed.h
 *
 * Z80 instruction table for unprefixed opcodes.
 *
 */

#include "Z80Instruction.h"
#include "Z80Nop.h"
#include "Z80LdAPtrBc.h"
#include "Z80LdRegByte.h"
#include "Z80LdRegPtrHl.h"
#include "Z80LdRegReg.h"

#include "Z80LdPtrHlByte.h"
#include "Z80LdPtrHlReg.h"

#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

class Z80Unprefixed
{
    public:
        // Instructions
        Z80Nop iNop;
        Z80LdAPtrBc iLdAPtrBc;
        Z80LdRegByte iLdRegByte;
        Z80LdRegPtrHl iLdRegPtrHl;
        Z80LdRegReg iLdRegReg;
        Z80LdPtrHlByte iLdPtrHlByte;
        Z80LdPtrHlReg iLdPtrHlReg;

        Z80PrefixDD iPrefixDD;
        Z80PrefixED iPrefixED;
        Z80PrefixFD iPrefixFD;

        Z80Instruction* table[4][8][8];

        Z80Unprefixed() :
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
                        &iLdRegByte,    // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegByte,    // 00001110: LD C, n
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
                        &iLdRegByte,    // 00010110: LD D, n
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
                        &iLdRegByte,    // 00011110: LD E, n
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
                        &iLdRegByte,    // 00100110: LD H, n
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
                        &iLdRegByte,    // 00101110: LD L, n
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
                        &iLdPtrHlByte,  // 00110110: LD (HL), n
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
                        &iLdRegByte,    // 00111110: LD A, n
                        &iNop
                    }
                },
                // x = 1
                {
                    // y = 0
                    {
                        &iLdRegReg,     // 01000000: LD B, B
                        &iLdRegReg,     // 01000001: LD B, C
                        &iLdRegReg,     // 01000010: LD B, D
                        &iLdRegReg,     // 01000011: LD B, E
                        &iLdRegReg,     // 01000100: LD B, H
                        &iLdRegReg,     // 01000101: LD B, L
                        &iLdRegPtrHl,   // 01000110: LD B, (HL)
                        &iLdRegReg      // 01000111: LD B, A
                    },
                    // y = 1
                    {
                        &iLdRegReg,     // 01001000: LD C, B
                        &iLdRegReg,     // 01001001: LD C, C
                        &iLdRegReg,     // 01001010: LD C, D
                        &iLdRegReg,     // 01001011: LD C, E
                        &iLdRegReg,     // 01001100: LD C, H
                        &iLdRegReg,     // 01001101: LD C, L
                        &iLdRegPtrHl,   // 01001110: LD C, (HL)
                        &iLdRegReg      // 01001111: LD C, A
                    },
                    // y = 2
                    {
                        &iLdRegReg,     // 01010000: LD D, B
                        &iLdRegReg,     // 01010001: LD D, C
                        &iLdRegReg,     // 01010010: LD D, D
                        &iLdRegReg,     // 01010011: LD D, E
                        &iLdRegReg,     // 01010100: LD D, H
                        &iLdRegReg,     // 01010101: LD D, L
                        &iLdRegPtrHl,   // 01010110: LD D, (HL)
                        &iLdRegReg      // 01010111: LD D, A
                    },
                    // y = 3
                    {
                        &iLdRegReg,     // 01011000: LD E, B
                        &iLdRegReg,     // 01011001: LD E, C
                        &iLdRegReg,     // 01011010: LD E, D
                        &iLdRegReg,     // 01011011: LD E, E
                        &iLdRegReg,     // 01011100: LD E, H
                        &iLdRegReg,     // 01011101: LD E, L
                        &iLdRegPtrHl,   // 01011110: LD E, (HL)
                        &iLdRegReg      // 01011111: LD E, A
                    },
                    // y = 4
                    {
                        &iLdRegReg,     // 01100000: LD H, B
                        &iLdRegReg,     // 01100001: LD H, C
                        &iLdRegReg,     // 01100010: LD H, D
                        &iLdRegReg,     // 01100011: LD H, E
                        &iLdRegReg,     // 01100100: LD H, H
                        &iLdRegReg,     // 01100101: LD H, L
                        &iLdRegPtrHl,   // 01100110: LD H, (HL)
                        &iLdRegReg      // 01100111: LD H, A
                    },
                    // y = 5
                    {
                        &iLdRegReg,     // 01101000: LD L, B
                        &iLdRegReg,     // 01101001: LD L, C
                        &iLdRegReg,     // 01101010: LD L, D
                        &iLdRegReg,     // 01101011: LD L, E
                        &iLdRegReg,     // 01101100: LD L, H
                        &iLdRegReg,     // 01101101: LD L, L
                        &iLdRegPtrHl,   // 01101110: LD L, (HL)
                        &iLdRegReg      // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iLdPtrHlReg,   // 01110000: LD (HL), B
                        &iLdPtrHlReg,   // 01110001: LD (HL), C
                        &iLdPtrHlReg,   // 01110010: LD (HL), D
                        &iLdPtrHlReg,   // 01110011: LD (HL), E
                        &iLdPtrHlReg,   // 01110100: LD (HL), H
                        &iLdPtrHlReg,   // 01110101: LD (HL), L
                        &iNop,          // 01110110: HALT
                        &iLdPtrHlReg    // 01110111: LD (HL), A
                    },
                    // y = 7
                    {
                        &iLdRegReg,     // 01111000: LD A, B
                        &iLdRegReg,     // 01111001: LD A, C
                        &iLdRegReg,     // 01111010: LD A, D
                        &iLdRegReg,     // 01111011: LD A, E
                        &iLdRegReg,     // 01111100: LD A, H
                        &iLdRegReg,     // 01111101: LD A, L
                        &iLdRegPtrHl,   // 01111110: LD A, (HL)
                        &iLdRegReg      // 01111111: LD A, A
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iPrefixDD, // 11011101: DD Prefix
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iPrefixED, // 11101101: ED Prefix
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop, // 00000000: NOP
                        &iNop, // 00000001: LD rp[p], nn
                        &iNop, // 00000010: LD (BC), A
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
                        &iPrefixFD, // 11111101: FD Prefix
                        &iNop,
                        &iNop
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
