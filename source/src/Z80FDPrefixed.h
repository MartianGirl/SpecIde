#pragma once

/** Z80FDPrefixed.h
 *
 * Z80 instruction table for FD-prefixed opcodes.
 *
 */

#include "Z80Instruction.h"
#include "Z80Nop.h"

#include "Z80LdAPtrBc.h"
#include "Z80LdAPtrDe.h"
#include "Z80LdAPtrWord.h"

#include "Z80LdPtrBcA.h"
#include "Z80LdPtrDeA.h"
#include "Z80LdPtrWordA.h"

#include "Z80LdRegYByte.h"
#include "Z80LdRegPtrIy.h"
#include "Z80LdRegYRegY.h"

#include "Z80LdPtrIyByte.h"
#include "Z80LdPtrIyReg.h"

#include "Z80LdRegWord.h"
#include "Z80LdIyWord.h"
#include "Z80LdIyPtrWord.h"
#include "Z80LdPtrWordIy.h"
#include "Z80LdSpIy.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"

#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

class Z80FDPrefixed
{
    public:
        // Instructions
        Z80Nop iNop; 

        Z80LdAPtrBc iLdAPtrBc;
        Z80LdAPtrDe iLdAPtrDe;
        Z80LdAPtrWord iLdAPtrWord;
        Z80LdPtrBcA iLdPtrBcA;
        Z80LdPtrDeA iLdPtrDeA;
        Z80LdPtrWordA iLdPtrWordA;
        Z80LdRegYByte iLdRegYByte;
        Z80LdRegPtrIy iLdRegPtrIy;
        Z80LdRegYRegY iLdRegYRegY;
        Z80LdPtrIyByte iLdPtrIyByte;
        Z80LdPtrIyReg iLdPtrIyReg;

        Z80LdRegWord iLdRegWord;
        Z80LdIyWord iLdIyWord;
        Z80LdIyPtrWord iLdIyPtrWord;
        Z80LdPtrWordIy iLdPtrWordIy;
        Z80LdSpIy iLdSpIy;
        Z80PushReg iPushReg;
        Z80PopReg iPopReg;

        Z80PrefixDD iPrefixDD;
        Z80PrefixED iPrefixED;
        Z80PrefixFD iPrefixFD;

        Z80Instruction* table[4][8][8];

        Z80FDPrefixed() :
            table {
                // x = 0
                {
                    // y = 0
                    {
                        &iNop,
                        &iLdRegWord,    // 00000001: LD BC, nn
                        &iLdPtrBcA,     // 00000010: LD (BC), A
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00000110: LD B, n
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
                        &iLdRegYByte,   // 00001110: LD C, n
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iLdRegWord,    // 00010001: LD DE, nn
                        &iLdPtrDeA,     // 00010010: LD (DE), A
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00010110: LD D, n
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrDe,     // 00011010: LD A, (DE)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iLdIyWord,     // 00100001: LD IY, nn
                        &iLdPtrWordIy,  // 00100010: LD (nn), IY
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00100110: LD IYh, n
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iLdIyPtrWord,  // 00101010: LD IY, (nn)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00101110: LD IYl, n
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iLdRegWord,    // 00110001: LD SP, nn
                        &iLdPtrWordA,   // 00110010: LD (nn), A
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdPtrIyByte,  // 00110110: LD (IY + d), n
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrWord,   // 00111010: LD A, (nn)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegYByte,   // 00111110: LD A, n
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
                        &iLdRegPtrIy,   // 01000110: LD B, (IY + d)
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
                        &iLdRegPtrIy,   // 01001110: LD C, (IY + d)
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
                        &iLdRegPtrIy,   // 01010110: LD D, (IY + d)
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
                        &iLdRegPtrIy,   // 01011110: LD E, (IY + d)
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
                        &iLdRegPtrIy,   // 01100110: LD H, (IY + d)
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
                        &iLdRegPtrIy,   // 01101110: LD L, (IY + d)
                        &iLdRegYRegY    // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iLdPtrIyReg,   // 01110000: LD (IY + d), B
                        &iLdPtrIyReg,   // 01110001: LD (IY + d), C
                        &iLdPtrIyReg,   // 01110010: LD (IY + d), D
                        &iLdPtrIyReg,   // 01110011: LD (IY + d), E
                        &iLdPtrIyReg,   // 01110100: LD (IY + d), H
                        &iLdPtrIyReg,   // 01110101: LD (IY + d), L
                        &iNop,          // 01110110: HALT
                        &iLdPtrIyReg    // 01110111: LD (IY + d), A
                    },
                    // y = 7
                    {
                        &iLdRegYRegY,   // 01111000: LD A, B
                        &iLdRegYRegY,   // 01111001: LD A, C
                        &iLdRegYRegY,   // 01111010: LD A, D
                        &iLdRegYRegY,   // 01111011: LD A, E
                        &iLdRegYRegY,   // 01111100: LD A, IYh
                        &iLdRegYRegY,   // 01111101: LD A, IYl
                        &iLdRegPtrIy,   // 01111110: LD A, (IY + d)
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
                        &iPopReg,   // 11000001: POP BC
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,  // 11000101: PUSH BC
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
                        &iPopReg,   // 11010001: POP DE
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,  // 11010101: PUSH DE
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
                        &iPrefixED, // 11101101: ED Prefix
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iPopReg,   // 11110001: POP AF
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,  // 11110101: PUSH AF
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iLdSpIy,   // 11111001: LD SP, IY
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
