#pragma once

/** Z80EDPrefixed.h
 *
 * Z80 instruction table for ED-prefixed opcodes.
 *
 */

#include "Z80Instruction.h"
#include "Z80Nop.h"

#include "Z80LdAI.h"
#include "Z80LdAR.h"
#include "Z80LdIA.h"
#include "Z80LdRA.h"

#include "Z80LdRegPtrWord.h"
#include "Z80LdPtrWordReg.h"

#include "Z80Ldi.h"
#include "Z80Ldir.h"
#include "Z80Ldd.h"
#include "Z80Lddr.h"
#include "Z80Cpi.h"
#include "Z80Cpir.h"
#include "Z80Cpd.h"
#include "Z80Cpdr.h"

#include "Z80Neg.h"

#include "Z80Im.h"
#include "Z80RetI.h"

#include "Z80InRegPtrC.h"
#include "Z80OutPtrCReg.h"
#include "Z80Ini.h"
#include "Z80Inir.h"
#include "Z80Ind.h"
#include "Z80Indr.h"
#include "Z80Outi.h"
#include "Z80Otir.h"
#include "Z80Outd.h"
#include "Z80Otdr.h"

#include "Z80AdcHlReg.h"
#include "Z80SbcHlReg.h"


class Z80EDPrefixed
{
    public:
        // Instructions
        Z80Nop iNop; 

        Z80LdAI iLdAI;
        Z80LdAR iLdAR;
        Z80LdIA iLdIA;
        Z80LdRA iLdRA;

        Z80LdRegPtrWord iLdRegPtrWord;
        Z80LdPtrWordReg iLdPtrWordReg;

        Z80Ldi iLdi;
        Z80Ldir iLdir;
        Z80Ldd iLdd;
        Z80Lddr iLddr;
        Z80Cpi iCpi;
        Z80Cpir iCpir;
        Z80Cpd iCpd;
        Z80Cpdr iCpdr;

        Z80Neg iNeg;

        Z80Im iIm;
        Z80RetI iRetI;

        Z80InRegPtrC iInRegPtrC;
        Z80OutPtrCReg iOutPtrCReg;
        Z80Ini iIni;
        Z80Inir iInir;
        Z80Ind iInd;
        Z80Indr iIndr;
        Z80Outi iOuti;
        Z80Otir iOtir;
        Z80Outd iOutd;
        Z80Otdr iOtdr;

        Z80AdcHlReg iAdcHlReg;
        Z80SbcHlReg iSbcHlReg;

        Z80Instruction* table[4][8][8];

        Z80EDPrefixed() :
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
                        &iInRegPtrC,    // 01000000: IN B, (C)
                        &iOutPtrCReg,   // 01000001: OUT (C), B
                        &iSbcHlReg,     // 01000010: SBC HL, BC
                        &iLdPtrWordReg, // 01000011: LD (nn), BC
                        &iNeg,          // 01000100: NEG
                        &iRetI,         // 01000101: RETN
                        &iIm,           // 01000110: IM 0
                        &iLdIA          // 01000111: LD I, A
                    },
                    // y = 1
                    {
                        &iInRegPtrC,    // 01001000: IN C, (C)
                        &iOutPtrCReg,   // 01001001: OUT (C), C
                        &iAdcHlReg,     // 01001010: ADC HL, BC
                        &iLdRegPtrWord, // 01001011: LD BC, (nn)
                        &iNop,
                        &iRetI,         // 01001101: RETI
                        &iIm,           // 01001110: IM 0*
                        &iLdRA          // 01001111: LD R, A
                    },
                    // y = 2
                    {
                        &iInRegPtrC,    // 01010000: IN D, (C)
                        &iOutPtrCReg,   // 01010001: OUT (C), D
                        &iSbcHlReg,     // 01010010: SBC HL, DE
                        &iLdPtrWordReg, // 01010011: LD (nn), DE
                        &iNop,
                        &iRetI,         // 01010101: RETN*
                        &iIm,           // 01010110: IM 1
                        &iLdAI          // 01010111: LD A, I
                    },
                    // y = 3
                    {
                        &iInRegPtrC,    // 01011000: IN E, (C)
                        &iOutPtrCReg,   // 01011001: OUT (C), E
                        &iAdcHlReg,     // 01011010: ADC HL, DE
                        &iLdRegPtrWord, // 01011011: LD DE, (nn)
                        &iNop,
                        &iRetI,         // 01011101: RETN*
                        &iIm,           // 01011110: IM 2
                        &iLdAR          // 01011111: LD A, R
                    },
                    // y = 4
                    {
                        &iInRegPtrC,    // 01100000: IN H, (C)
                        &iOutPtrCReg,   // 01100001: OUT (C), H
                        &iSbcHlReg,     // 01100010: SBC HL, HL
                        &iLdPtrWordReg, // 01100011: LD (nn), HL
                        &iNop,
                        &iRetI,         // 01100101: RETN*
                        &iIm,           // 01100110: IM 0*
                        &iNop
                    },
                    // y = 5
                    {
                        &iInRegPtrC,    // 01101000: IN L, (C)
                        &iOutPtrCReg,   // 01101001: OUT (C), L
                        &iAdcHlReg,     // 01101010: ADC HL, HL
                        &iLdRegPtrWord, // 01101011: LD HL, (nn)
                        &iNop,
                        &iRetI,         // 01101101: RETN*
                        &iIm,           // 01101110: IM 0*
                        &iNop
                    },
                    // y = 6
                    {
                        &iInRegPtrC,    // 01110000: IN F, (C)
                        &iOutPtrCReg,   // 01110001: OUT (C), 0
                        &iSbcHlReg,     // 01110010: SBC HL, SP
                        &iLdPtrWordReg, // 01110011: LD (nn), SP
                        &iNop,
                        &iRetI,         // 01110101: RETN*
                        &iIm,           // 01110110: IM 1*
                        &iNop
                    },
                    // y = 7
                    {
                        &iInRegPtrC,    // 01111000: IN A, (C)
                        &iOutPtrCReg,   // 01111001: OUT (C), A
                        &iAdcHlReg,     // 01111010: ADC HL, SP
                        &iLdRegPtrWord, // 01111011: LD SP, (nn)
                        &iNop,
                        &iRetI,         // 01111101: RETN*
                        &iIm,           // 01111110: IM 2*
                        &iNop
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
                        &iLdi,      // 10100000: LDI
                        &iCpi,      // 10100001: CPI
                        &iIni,      // 10100010: INI
                        &iOuti,     // 10100011: OUTI
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iLdd,      // 10101000: LDD
                        &iCpd,      // 10101001: CPD
                        &iInd,      // 10101010: IND
                        &iOutd,     // 10101011: OUTD
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iLdir,     // 10110000: LDIR
                        &iCpir,     // 10110001: CPIR
                        &iInir,     // 10110010: INIR
                        &iOtir,     // 10110011: OTIR
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iLddr,     // 10111000: LDDR
                        &iCpdr,     // 10111001: CPDR
                        &iIndr,     // 10111010: INDR
                        &iOtdr,     // 10111011: OTDR
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
