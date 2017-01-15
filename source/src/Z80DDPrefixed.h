#pragma once

/** Z80DDPrefixed.h
 *
 * Z80 instruction table for DD-prefixed opcodes.
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

#include "Z80LdRegXByte.h"
#include "Z80LdRegPtrIx.h"
#include "Z80LdRegXRegX.h"

#include "Z80LdPtrIxByte.h"
#include "Z80LdPtrIxReg.h"

#include "Z80LdRegWord.h"
#include "Z80LdIxWord.h"
#include "Z80LdIxPtrWord.h"
#include "Z80LdPtrWordIx.h"
#include "Z80LdSpIx.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"
#include "Z80PushIx.h"
#include "Z80PopIx.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpIx.h"

#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

class Z80DDPrefixed
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
        Z80LdRegXByte iLdRegXByte;
        Z80LdRegPtrIx iLdRegPtrIx;
        Z80LdRegXRegX iLdRegXRegX;
        Z80LdPtrIxByte iLdPtrIxByte;
        Z80LdPtrIxReg iLdPtrIxReg;

        Z80LdRegWord iLdRegWord;
        Z80LdIxWord iLdIxWord;
        Z80LdIxPtrWord iLdIxPtrWord;
        Z80LdPtrWordIx iLdPtrWordIx;
        Z80LdSpIx iLdSpIx;
        Z80PushReg iPushReg;
        Z80PopReg iPopReg;
        Z80PushIx iPushIx;
        Z80PopIx iPopIx;

        Z80ExDeHl iExDeHl;
        Z80ExAfAf iExAfAf;
        Z80Exx iExx;
        Z80ExPtrSpIx iExPtrSpIx;

        Z80PrefixDD iPrefixDD;
        Z80PrefixED iPrefixED;
        Z80PrefixFD iPrefixFD;

        Z80Instruction* table[4][8][8];

        Z80DDPrefixed() :
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
                        &iLdRegXByte,   // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iExAfAf,       // 00001000: EX AF, AF'
                        &iNop,
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegXByte,   // 00001110: LD C, n
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
                        &iLdRegXByte,   // 00010110: LD D, n
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
                        &iLdRegXByte,   // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iLdIxWord,     // 00100001: LD IX, nn
                        &iLdPtrWordIx,  // 00100010: LD (nn), IX
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegXByte,   // 00100110: LD IXh, n
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iLdIxPtrWord,  // 00101010: LD IX, (nn)
                        &iNop,
                        &iNop,
                        &iNop,
                        &iLdRegXByte,   // 00101110: LD IXl, n
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
                        &iLdPtrIxByte,  // 00110110: LD (IX + d), n
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
                        &iLdRegXByte,   // 00111110: LD A, n
                        &iNop
                    }
                },
                // x = 1
                {
                    // y = 0
                    {
                        &iLdRegXRegX,   // 01000000: LD B, B
                        &iLdRegXRegX,   // 01000001: LD B, C
                        &iLdRegXRegX,   // 01000010: LD B, D
                        &iLdRegXRegX,   // 01000011: LD B, E
                        &iLdRegXRegX,   // 01000100: LD B, IXh
                        &iLdRegXRegX,   // 01000101: LD B, IXl
                        &iLdRegPtrIx,   // 01000110: LD B, (IX + d)
                        &iLdRegXRegX    // 01000111: LD B, A
                    },
                    // y = 1
                    {
                        &iLdRegXRegX,   // 01001000: LD C, B
                        &iLdRegXRegX,   // 01001001: LD C, C
                        &iLdRegXRegX,   // 01001010: LD C, D
                        &iLdRegXRegX,   // 01001011: LD C, E
                        &iLdRegXRegX,   // 01001100: LD C, IXh
                        &iLdRegXRegX,   // 01001101: LD C, IXl
                        &iLdRegPtrIx,   // 01001110: LD C, (IX + d)
                        &iLdRegXRegX    // 01001111: LD C, A
                    },
                    // y = 2
                    {
                        &iLdRegXRegX,   // 01010000: LD D, B
                        &iLdRegXRegX,   // 01010001: LD D, C
                        &iLdRegXRegX,   // 01010010: LD D, D
                        &iLdRegXRegX,   // 01010011: LD D, E
                        &iLdRegXRegX,   // 01010100: LD D, IXh
                        &iLdRegXRegX,   // 01010101: LD D, IXl
                        &iLdRegPtrIx,   // 01010110: LD D, (IX + d)
                        &iLdRegXRegX    // 01010111: LD D, A
                    },
                    // y = 3
                    {
                        &iLdRegXRegX,   // 01011000: LD E, B
                        &iLdRegXRegX,   // 01011001: LD E, C
                        &iLdRegXRegX,   // 01011010: LD E, D
                        &iLdRegXRegX,   // 01011011: LD E, E
                        &iLdRegXRegX,   // 01011100: LD E, IXh
                        &iLdRegXRegX,   // 01011101: LD E, IXl
                        &iLdRegPtrIx,   // 01011110: LD E, (IX + d)
                        &iLdRegXRegX    // 01011111: LD E, A
                    },
                    // y = 4
                    {
                        &iLdRegXRegX,   // 01100000: LD H, B
                        &iLdRegXRegX,   // 01100001: LD H, C
                        &iLdRegXRegX,   // 01100010: LD H, D
                        &iLdRegXRegX,   // 01100011: LD H, E
                        &iLdRegXRegX,   // 01100100: LD H, IXh
                        &iLdRegXRegX,   // 01100101: LD H, IXl
                        &iLdRegPtrIx,   // 01100110: LD H, (IX + d)
                        &iLdRegXRegX    // 01100111: LD H, A
                    },
                    // y = 5
                    {
                        &iLdRegXRegX,   // 01101000: LD L, B
                        &iLdRegXRegX,   // 01101001: LD L, C
                        &iLdRegXRegX,   // 01101010: LD L, D
                        &iLdRegXRegX,   // 01101011: LD L, E
                        &iLdRegXRegX,   // 01101100: LD L, IXh
                        &iLdRegXRegX,   // 01101101: LD L, IXl
                        &iLdRegPtrIx,   // 01101110: LD L, (IX + d)
                        &iLdRegXRegX    // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iLdPtrIxReg,   // 01110000: LD (IX + d), B
                        &iLdPtrIxReg,   // 01110001: LD (IX + d), C
                        &iLdPtrIxReg,   // 01110010: LD (IX + d), D
                        &iLdPtrIxReg,   // 01110011: LD (IX + d), E
                        &iLdPtrIxReg,   // 01110100: LD (IX + d), H
                        &iLdPtrIxReg,   // 01110101: LD (IX + d), L
                        &iNop,          // 01110110: HALT
                        &iLdPtrIxReg    // 01110111: LD (IX + d), A
                    },
                    // y = 7
                    {
                        &iLdRegXRegX,   // 01111000: LD A, B
                        &iLdRegXRegX,   // 01111001: LD A, C
                        &iLdRegXRegX,   // 01111010: LD A, D
                        &iLdRegXRegX,   // 01111011: LD A, E
                        &iLdRegXRegX,   // 01111100: LD A, IXh
                        &iLdRegXRegX,   // 01111101: LD A, IXl
                        &iLdRegPtrIx,   // 01111110: LD A, (IX + d)
                        &iLdRegXRegX    // 01111111: LD A, A
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
                        &iPopReg,       // 11000001: POP BC
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,      // 11000101: PUSH BC
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
                        &iPopReg,       // 11010001: POP DE
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,      // 11010101: PUSH DE
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iExx,          // 11011001: EXX
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPrefixDD,     // 11011101: DD Prefix
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iPopIx,        // 11100001: POP IX
                        &iNop,
                        &iExPtrSpIx,    // 11100011: EX (SP), IX
                        &iNop,
                        &iPushIx,       // 11100101: PUSH IX
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iExDeHl,       // 11101011: EX DE, HL
                        &iNop,
                        &iPrefixED,     // 11101101: ED Prefix
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iPopReg,       // 11110001: POP AF
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPushReg,      // 11110101: PUSH AF
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iLdSpIx,       // 11111001: LD SP, IX
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPrefixFD,     // 11111101: FD Prefix
                        &iNop,
                        &iNop
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
