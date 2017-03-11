#pragma once

/** Z80Unprefixed.h
 *
 * Z80 instruction table for unprefixed opcodes.
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

#include "Z80LdRegByte.h"
#include "Z80LdRegPtrHl.h"
#include "Z80LdRegReg.h"

#include "Z80LdPtrHlByte.h"
#include "Z80LdPtrHlReg.h"

#include "Z80LdRegWord.h"
#include "Z80LdHlPtrWord.h"
#include "Z80LdPtrWordHl.h"
#include "Z80LdSpHl.h"
#include "Z80PushReg.h"
#include "Z80PopReg.h"

#include "Z80ExDeHl.h"
#include "Z80ExAfAf.h"
#include "Z80Exx.h"
#include "Z80ExPtrSpHl.h"

#include "Z80AddReg.h"
#include "Z80AdcReg.h"
#include "Z80SubReg.h"
#include "Z80SbcReg.h"
#include "Z80AndReg.h"
#include "Z80XorReg.h"
#include "Z80OrReg.h"
#include "Z80CpReg.h"
#include "Z80IncReg.h"
#include "Z80DecReg.h"

#include "Z80AddByte.h"
#include "Z80AdcByte.h"
#include "Z80SubByte.h"
#include "Z80SbcByte.h"
#include "Z80AndByte.h"
#include "Z80XorByte.h"
#include "Z80OrByte.h"
#include "Z80CpByte.h"

#include "Z80AddPtrHl.h"
#include "Z80AdcPtrHl.h"
#include "Z80SubPtrHl.h"
#include "Z80SbcPtrHl.h"
#include "Z80AndPtrHl.h"
#include "Z80XorPtrHl.h"
#include "Z80OrPtrHl.h"
#include "Z80CpPtrHl.h"
#include "Z80IncPtrHl.h"
#include "Z80DecPtrHl.h"

#include "Z80Daa.h"
#include "Z80Cpl.h"
#include "Z80Ccf.h"
#include "Z80Scf.h"

#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

class Z80Unprefixed
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
        Z80LdRegByte iLdRegByte;
        Z80LdRegPtrHl iLdRegPtrHl;
        Z80LdRegReg iLdRegReg;
        Z80LdPtrHlByte iLdPtrHlByte;
        Z80LdPtrHlReg iLdPtrHlReg;

        Z80LdRegWord iLdRegWord;
        Z80LdHlPtrWord iLdHlPtrWord;
        Z80LdPtrWordHl iLdPtrWordHl;
        Z80LdSpHl iLdSpHl;
        Z80PushReg iPushReg;
        Z80PopReg iPopReg;

        Z80ExDeHl iExDeHl;
        Z80ExAfAf iExAfAf;
        Z80Exx iExx;
        Z80ExPtrSpHl iExPtrSpHl;

        Z80AddReg iAddReg;
        Z80AdcReg iAdcReg;
        Z80SubReg iSubReg;
        Z80SbcReg iSbcReg;
        Z80AndReg iAndReg;
        Z80XorReg iXorReg;
        Z80OrReg iOrReg;
        Z80CpReg iCpReg;
        Z80IncReg iIncReg;
        Z80DecReg iDecReg;

        Z80AddByte iAddByte;
        Z80AdcByte iAdcByte;
        Z80SubByte iSubByte;
        Z80SbcByte iSbcByte;
        Z80AndByte iAndByte;
        Z80XorByte iXorByte;
        Z80OrByte iOrByte;
        Z80CpByte iCpByte;

        Z80AddPtrHl iAddPtrHl;
        Z80AdcPtrHl iAdcPtrHl;
        Z80SubPtrHl iSubPtrHl;
        Z80SbcPtrHl iSbcPtrHl;
        Z80AndPtrHl iAndPtrHl;
        Z80XorPtrHl iXorPtrHl;
        Z80OrPtrHl iOrPtrHl;
        Z80CpPtrHl iCpPtrHl;
        Z80IncPtrHl iIncPtrHl;
        Z80DecPtrHl iDecPtrHl;

        Z80Daa iDaa;
        Z80Cpl iCpl;
        Z80Ccf iCcf;
        Z80Scf iScf;

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
                        &iLdRegWord,    // 00000001: LD BC, nn
                        &iLdPtrBcA,     // 00000010: LD (BC), A
                        &iNop,
                        &iIncReg,       // 00000100: INC B
                        &iDecReg,       // 00000101: DEC B
                        &iLdRegByte,    // 00000110: LD B, n
                        &iNop
                    },
                    // y = 1
                    {
                        &iExAfAf,       // 00001000: EX AF, AF'
                        &iNop,
                        &iLdAPtrBc,     // 00001010: LD A, (BC)
                        &iNop,
                        &iIncReg,       // 00001100: INC C
                        &iDecReg,       // 00001101: DEC C
                        &iLdRegByte,    // 00001110: LD C, n
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iLdRegWord,    // 00010001: LD DE, nn
                        &iLdPtrDeA,     // 00010010: LD (DE), A
                        &iNop,
                        &iIncReg,       // 00010100: INC D
                        &iDecReg,       // 00010101: DEC D
                        &iLdRegByte,    // 00010110: LD D, n
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrDe,     // 00011010: LD A, (DE)
                        &iNop,
                        &iIncReg,       // 00011100: INC E
                        &iDecReg,       // 00011101: DEC E
                        &iLdRegByte,    // 00011110: LD E, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iLdRegWord,    // 00100001: LD HL, nn
                        &iLdPtrWordHl,  // 00100010: LD (nn), HL
                        &iNop,
                        &iIncReg,       // 00100100: INC H
                        &iDecReg,       // 00100101: DEC H
                        &iLdRegByte,    // 00100110: LD H, n
                        &iDaa           // 00100111: DAA
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iLdHlPtrWord,  // 00101010: LD HL, (nn)
                        &iNop,
                        &iIncReg,       // 00101100: INC L
                        &iDecReg,       // 00101101: DEC L
                        &iLdRegByte,    // 00101110: LD L, n
                        &iCpl           // 00101111: CPL
                    },
                    // y = 6
                    {
                        &iNop,
                        &iLdRegWord,    // 00110001: LD SP, nn
                        &iLdPtrWordA,   // 00110010: LD (nn), A
                        &iNop,
                        &iIncPtrHl,     // 00110100: INC (HL)
                        &iDecPtrHl,     // 00110101: DEC (HL)
                        &iLdPtrHlByte,  // 00110110: LD (HL), n
                        &iScf           // 00110111: SCF
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iLdAPtrWord,   // 00111010: LD A, (nn)
                        &iNop,
                        &iIncReg,       // 00111100: INC A
                        &iDecReg,       // 00111101: DEC A
                        &iLdRegByte,    // 00111110: LD A, n
                        &iCcf           // 00111111: CCF
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
                        &iAddReg,       // 10000000: ADD A, B
                        &iAddReg,       // 10000001: ADD A, C
                        &iAddReg,       // 10000010: ADD A, D
                        &iAddReg,       // 10000011: ADD A, E
                        &iAddReg,       // 10000100: ADD A, H
                        &iAddReg,       // 10000101: ADD A, L
                        &iAddPtrHl,     // 10000110: ADD A, (HL)
                        &iAddReg        // 10000111: ADD A, A
                    },
                    // y = 1
                    {
                        &iAdcReg,       // 10001000: ADC A, B
                        &iAdcReg,       // 10001001: ADC A, C
                        &iAdcReg,       // 10001010: ADC A, D
                        &iAdcReg,       // 10001011: ADC A, E
                        &iAdcReg,       // 10001100: ADC A, H
                        &iAdcReg,       // 10001101: ADC A, L
                        &iAdcPtrHl,     // 10001110: ADC A, (HL)
                        &iAdcReg        // 10001111: ADC A, A
                    },
                    // y = 2
                    {
                        &iSubReg,       // 10010000: SUB B
                        &iSubReg,       // 10010001: SUB C
                        &iSubReg,       // 10010010: SUB D
                        &iSubReg,       // 10010011: SUB E
                        &iSubReg,       // 10010100: SUB H
                        &iSubReg,       // 10010101: SUB L
                        &iSubPtrHl,     // 10010110: SUB (HL)
                        &iSubReg        // 10010111: SUB A
                    },
                    // y = 3
                    {
                        &iSbcReg,       // 10011000: SBC A, B
                        &iSbcReg,       // 10011001: SBC A, C
                        &iSbcReg,       // 10011010: SBC A, D
                        &iSbcReg,       // 10011011: SBC A, E
                        &iSbcReg,       // 10011100: SBC A, H
                        &iSbcReg,       // 10011101: SBC A, L
                        &iSbcPtrHl,     // 10011110: SBC A, (HL)
                        &iSbcReg        // 10011111: SBC A, A
                    },
                    // y = 4
                    {
                        &iAndReg,       // 10100000: AND B
                        &iAndReg,       // 10100001: AND C
                        &iAndReg,       // 10100010: AND D
                        &iAndReg,       // 10100011: AND E
                        &iAndReg,       // 10100100: AND H
                        &iAndReg,       // 10100101: AND L
                        &iAndPtrHl,     // 10100110: AND (HL)
                        &iAndReg        // 10100111: AND A
                    },
                    // y = 5
                    {
                        &iXorReg,       // 10101000: XOR B
                        &iXorReg,       // 10101001: XOR C
                        &iXorReg,       // 10101010: XOR D
                        &iXorReg,       // 10101011: XOR E
                        &iXorReg,       // 10101100: XOR H
                        &iXorReg,       // 10101101: XOR L
                        &iXorPtrHl,     // 10101110: XOR (HL)
                        &iXorReg        // 10101111: XOR A
                    },
                    // y = 6
                    {
                        &iOrReg,        // 10110000: OR B
                        &iOrReg,        // 10110001: OR C
                        &iOrReg,        // 10110010: OR D
                        &iOrReg,        // 10110011: OR E
                        &iOrReg,        // 10110100: OR H
                        &iOrReg,        // 10110101: OR L
                        &iOrPtrHl,      // 10110110: OR (HL)
                        &iOrReg         // 10110111: OR A
                    },
                    // y = 7
                    {
                        &iCpReg,        // 10111000: CP B
                        &iCpReg,        // 10111001: CP C
                        &iCpReg,        // 10111010: CP D
                        &iCpReg,        // 10111011: CP E
                        &iCpReg,        // 10111100: CP H
                        &iCpReg,        // 10111101: CP L
                        &iCpPtrHl,      // 10111110: CP (HL)
                        &iCpReg         // 10111111: CP A
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
                        &iAddByte,      // 11000110: ADD A, n
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
                        &iAdcByte,      // 11001110: ADC A, n
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
                        &iSubByte,      // 11010110: SUB n
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
                        &iSbcByte,      // 11011110: SBC A, n
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iPopReg,       // 11100001: POP HL
                        &iNop,
                        &iExPtrSpHl,    // 11100011: EX (SP), HL
                        &iNop,
                        &iPushReg,      // 11100101: PUSH HL
                        &iAndByte,      // 11100110: AND n
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
                        &iXorByte,      // 11101110: XOR n
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
                        &iOrByte,       // 11110110: OR n
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iLdSpHl,       // 11111001: LD SP, HL
                        &iNop,
                        &iNop,
                        &iNop,
                        &iPrefixFD,     // 11111101: FD Prefix
                        &iCpByte,       // 11111110: CP n
                        &iNop
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
