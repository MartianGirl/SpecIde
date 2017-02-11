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
#include "Z80PushIy.h"
#include "Z80PopIy.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpIy.h"

#include "Z80AddRegY.h"
#include "Z80AdcRegY.h"
#include "Z80SubRegY.h"
#include "Z80SbcRegY.h"
#include "Z80AndRegY.h"
#include "Z80XorRegY.h"
#include "Z80OrRegY.h"
#include "Z80CpRegY.h"
#include "Z80IncRegY.h"
#include "Z80DecRegY.h"

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
        Z80PushIy iPushIy;
        Z80PopIy iPopIy;

        Z80ExDeHl iExDeHl;
        Z80ExAfAf iExAfAf;
        Z80Exx iExx;
        Z80ExPtrSpIy iExPtrSpIy;

        Z80AddRegY iAddRegY;
        Z80AdcRegY iAdcRegY;
        Z80SubRegY iSubRegY;
        Z80SbcRegY iSbcRegY;
        Z80AndRegY iAndRegY;
        Z80XorRegY iXorRegY;
        Z80OrRegY iOrRegY;
        Z80CpRegY iCpRegY;
        Z80IncRegY iIncRegY;
        Z80DecRegY iDecRegY;

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
                        &iIncRegY,      // 00000100: INC B
                        &iDecRegY,      // 00000101: DEC B
                        &iLdRegYByte,   // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iExAfAf,       // 00001000: EX AF, AF'
                        &iNop,
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iNop,
                        &iIncRegY,      // 00001100: INC C
                        &iDecRegY,      // 00001101: DEC C
                        &iLdRegYByte,   // 00001110: LD C, n
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iLdRegWord,    // 00010001: LD DE, nn
                        &iLdPtrDeA,     // 00010010: LD (DE), A
                        &iNop,
                        &iIncRegY,      // 00010100: INC D
                        &iDecRegY,      // 00010101: DEC D
                        &iLdRegYByte,   // 00010110: LD D, n
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrDe,     // 00011010: LD A, (DE)
                        &iNop,
                        &iIncRegY,      // 00011100: INC E
                        &iDecRegY,      // 00011100: DEC E
                        &iLdRegYByte,   // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iLdIyWord,     // 00100001: LD IY, nn
                        &iLdPtrWordIy,  // 00100010: LD (nn), IY
                        &iNop,
                        &iIncRegY,      // 00100100: INC IYh
                        &iDecRegY,      // 00100101: DEC IYh
                        &iLdRegYByte,   // 00100110: LD IYh, n
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iLdIyPtrWord,  // 00101010: LD IY, (nn)
                        &iNop,
                        &iIncRegY,      // 00101100: INC IYl
                        &iDecRegY,      // 00101101: DEC IYl
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
                        &iIncRegY,      // 00111100: INC A
                        &iDecRegY,      // 00111101: DEC A
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
                        &iAddRegY,      // 10000000: ADD A, B
                        &iAddRegY,      // 10000001: ADD A, C
                        &iAddRegY,      // 10000010: ADD A, D
                        &iAddRegY,      // 10000011: ADD A, E
                        &iAddRegY,      // 10000100: ADD A, IYh
                        &iAddRegY,      // 10000101: ADD A, IYl
                        &iNop,
                        &iAddRegY       // 10000111: ADD A, A
                    },
                    // y = 1
                    {
                        &iAdcRegY,      // 10001000: ADC A, B
                        &iAdcRegY,      // 10001001: ADC A, C
                        &iAdcRegY,      // 10001010: ADC A, D
                        &iAdcRegY,      // 10001011: ADC A, E
                        &iAdcRegY,      // 10001100: ADC A, IYh
                        &iAdcRegY,      // 10001101: ADC A, IYl
                        &iNop,
                        &iAdcRegY       // 10001111: ADC A, A
                    },
                    // y = 2
                    {
                        &iSubRegY,      // 10010000: SUB B
                        &iSubRegY,      // 10010001: SUB C
                        &iSubRegY,      // 10010010: SUB D
                        &iSubRegY,      // 10010011: SUB E
                        &iSubRegY,      // 10010100: SUB IYh
                        &iSubRegY,      // 10010101: SUB IYl
                        &iNop,
                        &iSubRegY       // 10010111: SUB A
                    },
                    // y = 3
                    {
                        &iSbcRegY,      // 10011000: SBC B
                        &iSbcRegY,      // 10011001: SBC C
                        &iSbcRegY,      // 10011010: SBC D
                        &iSbcRegY,      // 10011011: SBC E
                        &iSbcRegY,      // 10011100: SBC IYh
                        &iSbcRegY,      // 10011101: SBC IYl
                        &iNop,
                        &iSbcRegY       // 10011111: SBC A
                    },
                    // y = 4
                    {
                        &iAndRegY,      // 10100000: AND B
                        &iAndRegY,      // 10100001: AND C
                        &iAndRegY,      // 10100010: AND D
                        &iAndRegY,      // 10100011: AND E
                        &iAndRegY,      // 10100100: AND IYh
                        &iAndRegY,      // 10100101: AND IYl
                        &iNop,
                        &iAndRegY       // 10100111: AND A
                    },
                    // y = 5
                    {
                        &iXorRegY,      // 10101000: XOR B
                        &iXorRegY,      // 10101001: XOR C
                        &iXorRegY,      // 10101010: XOR D
                        &iXorRegY,      // 10101011: XOR E
                        &iXorRegY,      // 10101100: XOR IYh
                        &iXorRegY,      // 10101101: XOR IYl
                        &iNop,
                        &iXorRegY       // 10101111: XOR A
                    },
                    // y = 6
                    {
                        &iOrRegY,       // 10110000: OR B
                        &iOrRegY,       // 10110001: OR C
                        &iOrRegY,       // 10110010: OR D
                        &iOrRegY,       // 10110011: OR E
                        &iOrRegY,       // 10110100: OR IYh
                        &iOrRegY,       // 10110101: OR IYl
                        &iNop,
                        &iOrRegY        // 10110111: OR A
                    },
                    // y = 7
                    {
                        &iCpRegY,       // 10111000: CP B
                        &iCpRegY,       // 10111001: CP C
                        &iCpRegY,       // 10111010: CP D
                        &iCpRegY,       // 10111011: CP E
                        &iCpRegY,       // 10111100: CP IYh
                        &iCpRegY,       // 10111101: CP IYl
                        &iNop,
                        &iCpRegY        // 10111111: CP A
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
                        &iPopIy,        // 11100001: POP IY
                        &iNop,
                        &iExPtrSpIy,    // 11100011: EX (SP), IY
                        &iNop,
                        &iPushIy,       // 11100101: PUSH IY
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
                        &iLdSpIy,       // 11111001: LD SP, IY
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
