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

#include "Z80AddRegX.h"
#include "Z80AdcRegX.h"
#include "Z80SubRegX.h"
#include "Z80SbcRegX.h"
#include "Z80AndRegX.h"
#include "Z80XorRegX.h"
#include "Z80OrRegX.h"
#include "Z80CpRegX.h"
#include "Z80IncRegX.h"
#include "Z80DecRegX.h"

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrIx.h"
#include "Z80AdcPtrIx.h"
#include "Z80SubPtrIx.h"
#include "Z80SbcPtrIx.h"
#include "Z80AndPtrIx.h"
#include "Z80XorPtrIx.h"
#include "Z80OrPtrIx.h"
#include "Z80CpPtrIx.h"
#include "Z80IncPtrIx.h"
#include "Z80DecPtrIx.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"

#include "Z80JpWord.h"
#include "Z80JpCcWord.h"
#include "Z80JrByte.h"
#include "Z80JrCcByte.h"
#include "Z80JpIx.h"
#include "Z80Djnz.h"

#include "Z80Call.h"
#include "Z80Ret.h"
#include "Z80CallCc.h"
#include "Z80RetCc.h"
#include "Z80Rst.h"

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

        Z80AddRegX iAddRegX;
        Z80AdcRegX iAdcRegX;
        Z80SubRegX iSubRegX;
        Z80SbcRegX iSbcRegX;
        Z80AndRegX iAndRegX;
        Z80XorRegX iXorRegX;
        Z80OrRegX iOrRegX;
        Z80CpRegX iCpRegX;
        Z80IncRegX iIncRegX;
        Z80DecRegX iDecRegX;

        Z80AddByte iAddByte;
        Z80AdcByte iAdcByte;
        Z80SubByte iSubByte;
        Z80SbcByte iSbcByte;
        Z80AndByte iAndByte;
        Z80XorByte iXorByte;
        Z80OrByte iOrByte;
        Z80CpByte iCpByte;

        Z80AddPtrIx iAddPtrIx;
        Z80AdcPtrIx iAdcPtrIx;
        Z80SubPtrIx iSubPtrIx;
        Z80SbcPtrIx iSbcPtrIx;
        Z80AndPtrIx iAndPtrIx;
        Z80XorPtrIx iXorPtrIx;
        Z80OrPtrIx iOrPtrIx;
        Z80CpPtrIx iCpPtrIx;
        Z80IncPtrIx iIncPtrIx;
        Z80DecPtrIx iDecPtrIx;

        Z80Daa iDaa;
        Z80Cpl iCpl;
        Z80Ccf iCcf;
        Z80Scf iScf;

        Z80JpWord iJpWord;
        Z80JpCcWord iJpCcWord;
        Z80JrByte iJrByte;
        Z80JrCcByte iJrCcByte;
        Z80JpIx iJpIx;
        Z80Djnz iDjnz;

        Z80Call iCall;
        Z80Ret iRet;
        Z80CallCc iCallCc;
        Z80RetCc iRetCc;
        Z80Rst iRst;

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
                        &iIncRegX,      // 00000100: INC B
                        &iDecRegX,      // 00000101: DEC B
                        &iLdRegXByte,   // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iExAfAf,       // 00001000: EX AF, AF'
                        &iNop,
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iNop,
                        &iIncRegX,      // 00001100: INC C
                        &iDecRegX,      // 00001101: DEC C
                        &iLdRegXByte,   // 00001110: LD C, n
                        &iNop
                    },
                    // y = 2
                    {
                        &iDjnz,         // 00010000: DJNZ n
                        &iLdRegWord,    // 00010001: LD DE, nn
                        &iLdPtrDeA,     // 00010010: LD (DE), A
                        &iNop,
                        &iIncRegX,      // 00010100: INC D
                        &iDecRegX,      // 00010101: DEC D
                        &iLdRegXByte,   // 00010110: LD D, n
                        &iNop
                    },
                    // y = 3
                    {
                        &iJrByte,       // 00011000: JR n
                        &iNop,
                        &iLdAPtrDe,     // 00011010: LD A, (DE)
                        &iNop,
                        &iIncRegX,      // 00011100: INC E
                        &iDecRegX,      // 00011101: DEC E
                        &iLdRegXByte,   // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iJrCcByte,     // 00100000: JR NZ, n
                        &iLdIxWord,     // 00100001: LD IX, nn
                        &iLdPtrWordIx,  // 00100010: LD (nn), IX
                        &iNop,
                        &iIncRegX,      // 00100100: INC IXh
                        &iDecRegX,      // 00100101: INC IXh
                        &iLdRegXByte,   // 00100110: LD IXh, n
                        &iDaa           // 00100111: DAA
                    },
                    // y = 5
                    {
                        &iJrCcByte,     // 00101000: JR Z, n
                        &iNop,
                        &iLdIxPtrWord,  // 00101010: LD IX, (nn)
                        &iNop,
                        &iIncRegX,      // 00101100: INC IXl
                        &iDecRegX,      // 00101101: DEC IXl
                        &iLdRegXByte,   // 00101110: LD IXl, n
                        &iCpl           // 00101111: CPL
                    },
                    // y = 6
                    {
                        &iJrCcByte,     // 00110000: JR NC, n
                        &iLdRegWord,    // 00110001: LD SP, nn
                        &iLdPtrWordA,   // 00110010: LD (nn), A
                        &iNop,
                        &iIncPtrIx,     // 00110100: INC (IX+d)
                        &iDecPtrIx,     // 00110101: DEC (IX+d)
                        &iLdPtrIxByte,  // 00110110: LD (IX+d), n
                        &iScf           // 00110111: SCF
                    },
                    // y = 7
                    {
                        &iJrCcByte,     // 00111000: JR C, n
                        &iNop,
                        &iLdAPtrWord,   // 00111010: LD A, (nn)
                        &iNop,
                        &iIncRegX,      // 00111100: INC A
                        &iDecRegX,      // 00111101: DEC A
                        &iLdRegXByte,   // 00111110: LD A, n
                        &iCcf           // 00111111: CCF
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
                        &iLdRegPtrIx,   // 01000110: LD B, (IX+d)
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
                        &iLdRegPtrIx,   // 01001110: LD C, (IX+d)
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
                        &iLdRegPtrIx,   // 01010110: LD D, (IX+d)
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
                        &iLdRegPtrIx,   // 01011110: LD E, (IX+d)
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
                        &iLdRegPtrIx,   // 01100110: LD H, (IX+d)
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
                        &iLdRegPtrIx,   // 01101110: LD L, (IX+d)
                        &iLdRegXRegX    // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iLdPtrIxReg,   // 01110000: LD (IX+d), B
                        &iLdPtrIxReg,   // 01110001: LD (IX+d), C
                        &iLdPtrIxReg,   // 01110010: LD (IX+d), D
                        &iLdPtrIxReg,   // 01110011: LD (IX+d), E
                        &iLdPtrIxReg,   // 01110100: LD (IX+d), H
                        &iLdPtrIxReg,   // 01110101: LD (IX+d), L
                        &iNop,          // 01110110: HALT
                        &iLdPtrIxReg    // 01110111: LD (IX+d), A
                    },
                    // y = 7
                    {
                        &iLdRegXRegX,   // 01111000: LD A, B
                        &iLdRegXRegX,   // 01111001: LD A, C
                        &iLdRegXRegX,   // 01111010: LD A, D
                        &iLdRegXRegX,   // 01111011: LD A, E
                        &iLdRegXRegX,   // 01111100: LD A, IXh
                        &iLdRegXRegX,   // 01111101: LD A, IXl
                        &iLdRegPtrIx,   // 01111110: LD A, (IX+d)
                        &iLdRegXRegX    // 01111111: LD A, A
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iAddRegX,      // 10000000: ADD A, B
                        &iAddRegX,      // 10000001: ADD A, C
                        &iAddRegX,      // 10000010: ADD A, D
                        &iAddRegX,      // 10000011: ADD A, E         
                        &iAddRegX,      // 10000100: ADD A, IXh
                        &iAddRegX,      // 10000101: ADD A, IXl
                        &iAddPtrIx,     // 10000110: ADD A, (IX+d)
                        &iAddRegX       // 10000111: ADD A, A
                    },
                    // y = 1
                    {
                        &iAdcRegX,      // 10001000: ADC A, B
                        &iAdcRegX,      // 10001001: ADC A, C
                        &iAdcRegX,      // 10001010: ADC A, D
                        &iAdcRegX,      // 10001011: ADC A, E
                        &iAdcRegX,      // 10001100: ADC A, IXh
                        &iAdcRegX,      // 10001101: ADC A, IXl
                        &iAdcPtrIx,     // 10001110: ADC A, (IX+d)
                        &iAdcRegX       // 10001111: ADC A, A
                    },
                    // y = 2
                    {
                        &iSubRegX,      // 10010000: SUB B
                        &iSubRegX,      // 10010001: SUB C
                        &iSubRegX,      // 10010010: SUB D
                        &iSubRegX,      // 10010011: SUB E
                        &iSubRegX,      // 10010100: SUB IXh
                        &iSubRegX,      // 10010101: SUB IXl
                        &iSubPtrIx,     // 10010110: SUB (IX+d)
                        &iSubRegX       // 10010111: SUB A
                    },
                    // y = 3
                    {
                        &iSbcRegX,      // 10011000: SBC A, B
                        &iSbcRegX,      // 10011001: SBC A, C
                        &iSbcRegX,      // 10011010: SBC A, D
                        &iSbcRegX,      // 10011011: SBC A, E
                        &iSbcRegX,      // 10011100: SBC A, IXh
                        &iSbcRegX,      // 10011101: SBC A, IXl
                        &iSbcPtrIx,     // 10011110: SBC A, (IX+d)
                        &iSbcRegX       // 10011111: SBC A, A
                    },
                    // y = 4
                    {
                        &iAndRegX,      // 10100000: AND B
                        &iAndRegX,      // 10100001: AND C
                        &iAndRegX,      // 10100010: AND D
                        &iAndRegX,      // 10100011: AND E
                        &iAndRegX,      // 10100100: AND IXh
                        &iAndRegX,      // 10100101: AND IXl
                        &iAndPtrIx,     // 10100110: AND (IX+d)
                        &iAndRegX       // 10100111: AND A
                    },
                    // y = 5
                    {
                        &iXorRegX,      // 10101000: XOR B
                        &iXorRegX,      // 10101001: XOR C
                        &iXorRegX,      // 10101010: XOR D
                        &iXorRegX,      // 10101011: XOR E
                        &iXorRegX,      // 10101100: XOR IXh
                        &iXorRegX,      // 10101101: XOR IXl
                        &iXorPtrIx,     // 10101110: XOR (IX+d)
                        &iXorRegX       // 10101111: XOR A
                    },
                    // y = 6
                    {
                        &iOrRegX,       // 10110000: OR B
                        &iOrRegX,       // 10110001: OR C
                        &iOrRegX,       // 10110010: OR D
                        &iOrRegX,       // 10110011: OR E
                        &iOrRegX,       // 10110100: OR IXh
                        &iOrRegX,       // 10110101: OR IXl
                        &iOrPtrIx,      // 10110110: OR (IX+d)
                        &iOrRegX        // 10110111: OR A
                    },
                    // y = 7
                    {
                        &iCpRegX,       // 10111000: CP B
                        &iCpRegX,       // 10111001: CP C
                        &iCpRegX,       // 10111010: CP D
                        &iCpRegX,       // 10111011: CP E
                        &iCpRegX,       // 10111100: CP IXh
                        &iCpRegX,       // 10111101: CP IXl
                        &iCpPtrIx,      // 10111110: CP (IX+d)
                        &iCpRegX        // 10111111: CP A
                    }
                },
                // x = 3
                {
                    // y = 0
                    {
                        &iRetCc,        // 11000000: RET NZ
                        &iPopReg,       // 11000001: POP BC
                        &iJpCcWord,     // 11000010: JP NZ, nn
                        &iJpWord,       // 11000011: JP nn
                        &iCallCc,       // 11000100: CALL NZ, nn
                        &iPushReg,      // 11000101: PUSH BC
                        &iAddByte,      // 11000110: ADD A, n
                        &iRst           // 11000111: RST 0h
                    },
                    // y = 1
                    {
                        &iRetCc,        // 11001000: RET Z
                        &iRet,          // 11001001: RET
                        &iJpCcWord,     // 11001010: JP Z, nn
                        &iNop,
                        &iCallCc,       // 11001100: CALL Z, nn
                        &iCall,         // 11001101: CALL nn
                        &iAdcByte,      // 11001110: ADC A, n
                        &iRst           // 11001111: RST 8h
                    },
                    // y = 2
                    {
                        &iRetCc,        // 11010000: RET NC
                        &iPopReg,       // 11010001: POP DE
                        &iJpCcWord,     // 11010010: JP NC, nn
                        &iNop,
                        &iCallCc,       // 11010100: CALL NC, nn
                        &iPushReg,      // 11010101: PUSH DE
                        &iSubByte,      // 11010110: SUB n
                        &iRst           // 11010111: RST 10h
                    },
                    // y = 3
                    {
                        &iRetCc,        // 11011000: RET C
                        &iExx,          // 11011001: EXX
                        &iJpCcWord,     // 11011010: JP C, nn
                        &iNop,
                        &iCallCc,       // 11011100: CALL C, nn
                        &iPrefixDD,     // 11011101: DD Prefix
                        &iSbcByte,      // 11011110: SBC A, n
                        &iRst           // 11011111: RST 18h
                    },
                    // y = 4
                    {
                        &iRetCc,        // 11100000: RET PO
                        &iPopIx,        // 11100001: POP IX
                        &iJpCcWord,     // 11100010: JP PO, nn
                        &iExPtrSpIx,    // 11100011: EX (SP), IX
                        &iCallCc,       // 11100100: CALL PO, nn
                        &iPushIx,       // 11100101: PUSH IX
                        &iAndByte,      // 11100110: AND n
                        &iRst           // 11100111: RST 20h
                    },
                    // y = 5
                    {
                        &iRetCc,        // 11101000: RET PE
                        &iJpIx,         // 11101001: JP (IX)
                        &iJpCcWord,     // 11101010: JP PE, nn
                        &iExDeHl,       // 11101011: EX DE, HL
                        &iCallCc,       // 11101100: CALL PE, nn
                        &iPrefixED,     // 11101101: ED Prefix
                        &iXorByte,      // 11101110: XOR n
                        &iRst           // 11101111: RST 28h
                    },
                    // y = 6
                    {
                        &iRetCc,        // 11110000: RET P
                        &iPopReg,       // 11110001: POP AF
                        &iJpCcWord,     // 11110010: JP P, nn
                        &iNop,
                        &iCallCc,       // 11110100: CALL P, nn
                        &iPushReg,      // 11110101: PUSH AF
                        &iOrByte,       // 11110110: OR n
                        &iRst           // 11110111: RST 30h
                    },
                    // y = 7
                    {
                        &iRetCc,        // 11111000: RET M
                        &iLdSpIx,       // 11111001: LD SP, IX
                        &iJpCcWord,     // 11111010: JP M, nn
                        &iNop,
                        &iCallCc,       // 11111100: CALL M, nn
                        &iPrefixFD,     // 11111101: FD Prefix
                        &iCpByte,       // 11111110: CP n
                        &iRst           // 11111111: RST 38h
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
