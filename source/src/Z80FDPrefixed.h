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

#include "Z80LdRegYWord.h"
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

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrIy.h"
#include "Z80AdcPtrIy.h"
#include "Z80SubPtrIy.h"
#include "Z80SbcPtrIy.h"
#include "Z80AndPtrIy.h"
#include "Z80XorPtrIy.h"
#include "Z80OrPtrIy.h"
#include "Z80CpPtrIy.h"
#include "Z80IncPtrIy.h"
#include "Z80DecPtrIy.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"
#include "Z80Halt.h"
#include "Z80Di.h"
#include "Z80Ei.h"

#include "Z80AddIyRegY.h"
#include "Z80IncRegPY.h"
#include "Z80DecRegPY.h"

#include "Z80JpWord.h"
#include "Z80JpCcWord.h"
#include "Z80JrByte.h"
#include "Z80JrCcByte.h"
#include "Z80JpIy.h"
#include "Z80Djnz.h"

#include "Z80Call.h"
#include "Z80Ret.h"
#include "Z80CallCc.h"
#include "Z80RetCc.h"
#include "Z80Rst.h"

#include "Z80InAPtrByte.h"
#include "Z80OutPtrByteA.h"

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

        Z80LdRegYWord iLdRegYWord;
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

        Z80AddByte iAddByte;
        Z80AdcByte iAdcByte;
        Z80SubByte iSubByte;
        Z80SbcByte iSbcByte;
        Z80AndByte iAndByte;
        Z80XorByte iXorByte;
        Z80OrByte iOrByte;
        Z80CpByte iCpByte;

        Z80AddPtrIy iAddPtrIy;
        Z80AdcPtrIy iAdcPtrIy;
        Z80SubPtrIy iSubPtrIy;
        Z80SbcPtrIy iSbcPtrIy;
        Z80AndPtrIy iAndPtrIy;
        Z80XorPtrIy iXorPtrIy;
        Z80OrPtrIy iOrPtrIy;
        Z80CpPtrIy iCpPtrIy;
        Z80IncPtrIy iIncPtrIy;
        Z80DecPtrIy iDecPtrIy;

        Z80Daa iDaa;
        Z80Cpl iCpl;
        Z80Ccf iCcf;
        Z80Scf iScf;
        Z80Halt iHalt;
        Z80Di iDi;
        Z80Ei iEi;

        Z80AddIyRegY iAddIyRegY;
        Z80IncRegPY iIncRegPY;
        Z80DecRegPY iDecRegPY;

        Z80JpWord iJpWord;
        Z80JpCcWord iJpCcWord;
        Z80JrByte iJrByte;
        Z80JrCcByte iJrCcByte;
        Z80JpIy iJpIy;
        Z80Djnz iDjnz;

        Z80Call iCall;
        Z80Ret iRet;
        Z80CallCc iCallCc;
        Z80RetCc iRetCc;
        Z80Rst iRst;

        Z80InAPtrByte iInAPtrByte;
        Z80OutPtrByteA iOutPtrByteA;

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
                        &iNop,          // 00000000: NOP
                        &iLdRegYWord,   // 00000001: LD BC, nn
                        &iLdPtrBcA,     // 00000010: LD (BC), A
                        &iIncRegPY,     // 00000011: INC BC
                        &iIncRegY,      // 00000100: INC B
                        &iDecRegY,      // 00000101: DEC B
                        &iLdRegYByte,   // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iExAfAf,       // 00001000: EX AF, AF'
                        &iAddIyRegY,    // 00001001: ADD IY, BC
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iDecRegPY,     // 00001011: DEC BC
                        &iIncRegY,      // 00001100: INC C
                        &iDecRegY,      // 00001101: DEC C
                        &iLdRegYByte,   // 00001110: LD C, n
                        &iNop
                    },
                    // y = 2
                    {
                        &iDjnz,         // 00010000: DJNZ n
                        &iLdRegYWord,   // 00010001: LD DE, nn
                        &iLdPtrDeA,     // 00010010: LD (DE), A
                        &iIncRegPY,     // 00010011: INC DE
                        &iIncRegY,      // 00010100: INC D
                        &iDecRegY,      // 00010101: DEC D
                        &iLdRegYByte,   // 00010110: LD D, n
                        &iNop
                    },
                    // y = 3
                    {
                        &iJrByte,       // 00011000: JR n
                        &iAddIyRegY,    // 00011001: ADD IY, DE
                        &iLdAPtrDe,     // 00011010: LD A, (DE)
                        &iDecRegPY,     // 00011011: DEC DE
                        &iIncRegY,      // 00011100: INC E
                        &iDecRegY,      // 00011100: DEC E
                        &iLdRegYByte,   // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iJrCcByte,     // 00100000: JR NZ, n
                        &iLdRegYWord,   // 00100001: LD IY, nn
                        &iLdPtrWordIy,  // 00100010: LD (nn), IY
                        &iIncRegPY,     // 00100011: INC IY
                        &iIncRegY,      // 00100100: INC IYh
                        &iDecRegY,      // 00100101: DEC IYh
                        &iLdRegYByte,   // 00100110: LD IYh, n
                        &iDaa           // 00100111: DAA
                    },
                    // y = 5
                    {
                        &iJrCcByte,     // 00101000: JR Z, n
                        &iAddIyRegY,    // 00101001: ADD IY, IY
                        &iLdIyPtrWord,  // 00101010: LD IY, (nn)
                        &iDecRegPY,     // 00101011: DEC IY
                        &iIncRegY,      // 00101100: INC IYl
                        &iDecRegY,      // 00101101: DEC IYl
                        &iLdRegYByte,   // 00101110: LD IYl, n
                        &iCpl           // 00101111: CPL
                    },
                    // y = 6
                    {
                        &iJrCcByte,     // 00110000: JR NC, n
                        &iLdRegYWord,   // 00110001: LD SP, nn
                        &iLdPtrWordA,   // 00110010: LD (nn), A
                        &iIncRegPY,     // 00110011: INC SP
                        &iIncPtrIy,     // 00110100: INC (IY+d)
                        &iDecPtrIy,     // 00110101: DEC (IY+d)
                        &iLdPtrIyByte,  // 00110110: LD (IY+d), n
                        &iScf           // 00110111: SCF
                    },
                    // y = 7
                    {
                        &iJrCcByte,     // 00111000: JR C, n
                        &iAddIyRegY,    // 00111001: ADD IY, SP
                        &iLdAPtrWord,   // 00111010: LD A, (nn)
                        &iDecRegPY,     // 00111011: DEC SP
                        &iIncRegY,      // 00111100: INC A
                        &iDecRegY,      // 00111101: DEC A
                        &iLdRegYByte,   // 00111110: LD A, n
                        &iCcf           // 00111111: CCF
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
                        &iLdRegPtrIy,   // 01000110: LD B, (IY+d)
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
                        &iLdRegPtrIy,   // 01001110: LD C, (IY+d)
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
                        &iLdRegPtrIy,   // 01010110: LD D, (IY+d)
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
                        &iLdRegPtrIy,   // 01011110: LD E, (IY+d)
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
                        &iLdRegPtrIy,   // 01100110: LD H, (IY+d)
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
                        &iLdRegPtrIy,   // 01101110: LD L, (IY+d)
                        &iLdRegYRegY    // 01101111: LD L, A
                    },
                    // y = 6
                    {
                        &iLdPtrIyReg,   // 01110000: LD (IY+d), B
                        &iLdPtrIyReg,   // 01110001: LD (IY+d), C
                        &iLdPtrIyReg,   // 01110010: LD (IY+d), D
                        &iLdPtrIyReg,   // 01110011: LD (IY+d), E
                        &iLdPtrIyReg,   // 01110100: LD (IY+d), H
                        &iLdPtrIyReg,   // 01110101: LD (IY+d), L
                        &iHalt,         // 01110110: HALT
                        &iLdPtrIyReg    // 01110111: LD (IY+d), A
                    },
                    // y = 7
                    {
                        &iLdRegYRegY,   // 01111000: LD A, B
                        &iLdRegYRegY,   // 01111001: LD A, C
                        &iLdRegYRegY,   // 01111010: LD A, D
                        &iLdRegYRegY,   // 01111011: LD A, E
                        &iLdRegYRegY,   // 01111100: LD A, IYh
                        &iLdRegYRegY,   // 01111101: LD A, IYl
                        &iLdRegPtrIy,   // 01111110: LD A, (IY+d)
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
                        &iAddPtrIy,     // 10000110: ADD A, (IY+d)
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
                        &iAdcPtrIy,     // 10001110: ADC A, (IY+d)
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
                        &iSubPtrIy,     // 10010110: SUB (IY+d)
                        &iSubRegY       // 10010111: SUB A
                    },
                    // y = 3
                    {
                        &iSbcRegY,      // 10011000: SBC A, B
                        &iSbcRegY,      // 10011001: SBC A, C
                        &iSbcRegY,      // 10011010: SBC A, D
                        &iSbcRegY,      // 10011011: SBC A, E
                        &iSbcRegY,      // 10011100: SBC A, IYh
                        &iSbcRegY,      // 10011101: SBC A, IYl
                        &iSbcPtrIy,     // 10011110: SBC A, (IY+d)
                        &iSbcRegY       // 10011111: SBC A, A
                    },
                    // y = 4
                    {
                        &iAndRegY,      // 10100000: AND B
                        &iAndRegY,      // 10100001: AND C
                        &iAndRegY,      // 10100010: AND D
                        &iAndRegY,      // 10100011: AND E
                        &iAndRegY,      // 10100100: AND IYh
                        &iAndRegY,      // 10100101: AND IYl
                        &iAndPtrIy,     // 10100110: AND (IY+d)
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
                        &iXorPtrIy,     // 10101110: XOR (IY+d)
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
                        &iOrPtrIy,      // 10110110: OR (IY+d)
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
                        &iCpPtrIy,      // 10111110: CP (IY+d)
                        &iCpRegY        // 10111111: CP A
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
                        &iOutPtrByteA,  // 11010011: OUT (n), A
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
                        &iInAPtrByte,   // 11011011: IN A, (n)
                        &iCallCc,       // 11011100: CALL C, nn
                        &iPrefixDD,     // 11011101: DD Prefix
                        &iSbcByte,      // 11011110: SBC A, n
                        &iRst           // 11011111: RST 18h
                    },
                    // y = 4
                    {
                        &iRetCc,        // 11100000: RET PO
                        &iPopIy,        // 11100001: POP IY
                        &iJpCcWord,     // 11100010: JP PO, nn
                        &iExPtrSpIy,    // 11100011: EX (SP), IY
                        &iCallCc,       // 11100100: CALL PO, nn
                        &iPushIy,       // 11100101: PUSH IY
                        &iAndByte,      // 11100110: AND n
                        &iRst           // 11100111: RST 20h
                    },
                    // y = 5
                    {
                        &iRetCc,        // 11101000: RET PE
                        &iJpIy,         // 11101001: JP (IY)
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
                        &iDi,           // 11110011: DI
                        &iCallCc,       // 11110100: CALL P, nn
                        &iPushReg,      // 11110101: PUSH AF
                        &iOrByte,       // 11110110: OR n
                        &iRst           // 11110111: RST 30h
                    },
                    // y = 7
                    {
                        &iRetCc,        // 11111000: RET M
                        &iLdSpIy,       // 11111001: LD SP, IY
                        &iJpCcWord,     // 11111010: JP M, nn
                        &iEi,           // 11111011: EI
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
