edprefixed
{
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80InRegPtrC,       // 01000000: IN B, (C)
    &Z80::z80OutPtrCReg,      // 01000001: OUT (C), B
    &Z80::z80SbcHlReg,        // 01000010: SBC HL, BC
    &Z80::z80LdPtrWordReg,    // 01000011: LD (nn), BC
    &Z80::z80Neg,             // 01000100: NEG
    &Z80::z80RetI,            // 01000101: RETN
    &Z80::z80Im,              // 01000110: IM 0
    &Z80::z80LdIA,            // 01000111: LD I, A
    &Z80::z80InRegPtrC,       // 01001000: IN C, (C)
    &Z80::z80OutPtrCReg,      // 01001001: OUT (C), C
    &Z80::z80AdcHlReg,        // 01001010: ADC HL, BC
    &Z80::z80LdRegPtrWord,    // 01001011: LD BC, (nn)
    &Z80::z80Neg,             // 01001100: NEG*
    &Z80::z80RetI,            // 01001101: RETI
    &Z80::z80Im,              // 01001110: IM 0*
    &Z80::z80LdRA,            // 01001111: LD R, A
    &Z80::z80InRegPtrC,       // 01010000: IN D, (C)
    &Z80::z80OutPtrCReg,      // 01010001: OUT (C), D
    &Z80::z80SbcHlReg,        // 01010010: SBC HL, DE
    &Z80::z80LdPtrWordReg,    // 01010011: LD (nn), DE
    &Z80::z80Neg,             // 01010100: NEG*
    &Z80::z80RetI,            // 01010101: RETN*
    &Z80::z80Im,              // 01010110: IM 1
    &Z80::z80LdAI,            // 01010111: LD A, I
    &Z80::z80InRegPtrC,       // 01011000: IN E, (C)
    &Z80::z80OutPtrCReg,      // 01011001: OUT (C), E
    &Z80::z80AdcHlReg,        // 01011010: ADC HL, DE
    &Z80::z80LdRegPtrWord,    // 01011011: LD DE, (nn)
    &Z80::z80Neg,             // 01011100: NEG*
    &Z80::z80RetI,            // 01011101: RETN*
    &Z80::z80Im,              // 01011110: IM 2
    &Z80::z80LdAR,            // 01011111: LD A, R
    &Z80::z80InRegPtrC,       // 01100000: IN H, (C)
    &Z80::z80OutPtrCReg,      // 01100001: OUT (C), H
    &Z80::z80SbcHlReg,        // 01100010: SBC HL, HL
    &Z80::z80LdPtrWordReg,    // 01100011: LD (nn), HL
    &Z80::z80Neg,             // 01100100: NEG*
    &Z80::z80RetI,            // 01100101: RETN*
    &Z80::z80Im,              // 01100110: IM 0*
    &Z80::z80Rrd,             // 01100111: RRD
    &Z80::z80InRegPtrC,       // 01101000: IN L, (C)
    &Z80::z80OutPtrCReg,      // 01101001: OUT (C), L
    &Z80::z80AdcHlReg,        // 01101010: ADC HL, HL
    &Z80::z80LdRegPtrWord,    // 01101011: LD HL, (nn)
    &Z80::z80Neg,             // 01101100: NEG
    &Z80::z80RetI,            // 01101101: RETN*
    &Z80::z80Im,              // 01101110: IM 0*
    &Z80::z80Rld,             // 01101111: RLD
    &Z80::z80InRegPtrC,       // 01110000: IN F, (C)
    &Z80::z80OutPtrCReg,      // 01110001: OUT (C), 0
    &Z80::z80SbcHlReg,        // 01110010: SBC HL, SP
    &Z80::z80LdPtrWordReg,    // 01110011: LD (nn), SP
    &Z80::z80Neg,             // 01110100: NEG
    &Z80::z80RetI,            // 01110101: RETN*
    &Z80::z80Im,              // 01110110: IM 1*
    &Z80::z80Nop,             // 01110111: NOP*
    &Z80::z80InRegPtrC,       // 01111000: IN A, (C)
    &Z80::z80OutPtrCReg,      // 01111001: OUT (C), A
    &Z80::z80AdcHlReg,        // 01111010: ADC HL, SP
    &Z80::z80LdRegPtrWord,    // 01111011: LD SP, (nn)
    &Z80::z80Neg,             // 01111100: NEG*
    &Z80::z80RetI,            // 01111101: RETN*
    &Z80::z80Im,              // 01111110: IM 2*
    &Z80::z80Nop,             // 01111111: NOP*
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Ldi,         // 10100000: LDI
    &Z80::z80Cpi,         // 10100001: CPI
    &Z80::z80Ini,         // 10100010: INI
    &Z80::z80Outi,        // 10100011: OUTI
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Ldd,         // 10101000: LDD
    &Z80::z80Cpd,         // 10101001: CPD
    &Z80::z80Ind,         // 10101010: IND
    &Z80::z80Outd,        // 10101011: OUTD
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Ldir,        // 10110000: LDIR
    &Z80::z80Cpir,        // 10110001: CPIR
    &Z80::z80Inir,        // 10110010: INIR
    &Z80::z80Otir,        // 10110011: OTIR
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Lddr,        // 10111000: LDDR
    &Z80::z80Cpdr,        // 10111001: CPDR
    &Z80::z80Indr,        // 10111010: INDR
    &Z80::z80Otdr,        // 10111011: OTDR
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop,
    &Z80::z80Nop
},
