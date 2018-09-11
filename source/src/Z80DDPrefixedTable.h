/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

ddprefixed
{
    &Z80::z80Nop,                     // 00000000: NOP
    &Z80::z80LdRegXWord,              // 00000001: LD BC, nn
    &Z80::z80LdPtrBcA,                // 00000010: LD (BC), A
    &Z80::z80IncRegPX,                // 00000011: INC BC
    &Z80::z80IncRegX,                 // 00000100: INC B
    &Z80::z80DecRegX,                 // 00000101: DEC B
    &Z80::z80LdRegXByte,              // 00000110: LD B, n
    &Z80::z80Rlca,                    // 00000111: RLCA
    &Z80::z80ExAfAf,                  // 00001000: EX AF, AF'
    &Z80::z80AddIxRegX,               // 00001001: ADD IX, BC
    &Z80::z80LdAPtrBc,                // 00001010: LD A, (BC)
    &Z80::z80DecRegPX,                // 00001011: DEC BC
    &Z80::z80IncRegX,                 // 00001100: INC C
    &Z80::z80DecRegX,                 // 00001101: DEC C
    &Z80::z80LdRegXByte,              // 00001110: LD C, n
    &Z80::z80Rrca,                    // 00001111: RRCA
    &Z80::z80Djnz,                    // 00010000: DJNZ n
    &Z80::z80LdRegXWord,              // 00010001: LD DE, nn
    &Z80::z80LdPtrDeA,                // 00010010: LD (DE), A
    &Z80::z80IncRegPX,                // 00010011: INC DE
    &Z80::z80IncRegX,                 // 00010100: INC D
    &Z80::z80DecRegX,                 // 00010101: DEC D
    &Z80::z80LdRegXByte,              // 00010110: LD D, n
    &Z80::z80Rla,                     // 00010111: RLA
    &Z80::z80JrByte,                  // 00011000: JR n
    &Z80::z80AddIxRegX,               // 00011001: ADD IX, DE
    &Z80::z80LdAPtrDe,                // 00011010: LD A, (DE)
    &Z80::z80DecRegPX,                // 00011011: DEC DE
    &Z80::z80IncRegX,                 // 00011100: INC E
    &Z80::z80DecRegX,                 // 00011101: DEC E
    &Z80::z80LdRegXByte,              // 00011110: LD E, n
    &Z80::z80Rra,                     // 00011111: RRA
    &Z80::z80JrCcByte,                // 00100000: JR NZ, n
    &Z80::z80LdRegXWord,              // 00100001: LD IX, nn
    &Z80::z80LdPtrWordIx,             // 00100010: LD (nn), IX
    &Z80::z80IncRegPX,                // 00100011: INC IX
    &Z80::z80IncRegX,                 // 00100100: INC IXh
    &Z80::z80DecRegX,                 // 00100101: DEC IXh
    &Z80::z80LdRegXByte,              // 00100110: LD IXh, n
    &Z80::z80Daa,                     // 00100111: DAA
    &Z80::z80JrCcByte,                // 00101000: JR Z, n
    &Z80::z80AddIxRegX,               // 00101001: ADD IX, IX
    &Z80::z80LdIxPtrWord,             // 00101010: LD IX, (nn)
    &Z80::z80DecRegPX,                // 00101011: DEC IX
    &Z80::z80IncRegX,                 // 00101100: INC IXl
    &Z80::z80DecRegX,                 // 00101101: DEC IXl
    &Z80::z80LdRegXByte,              // 00101110: LD IXl, n
    &Z80::z80Cpl,                     // 00101111: CPL
    &Z80::z80JrCcByte,                // 00110000: JR NC, n
    &Z80::z80LdRegXWord,              // 00110001: LD SP, nn
    &Z80::z80LdPtrWordA,              // 00110010: LD (nn), A
    &Z80::z80IncRegPX,                // 00110011: INC SP
    &Z80::z80IncPtrIx,                // 00110100: INC (IX+d)
    &Z80::z80DecPtrIx,                // 00110101: DEC (IX+d)
    &Z80::z80LdPtrIxByte,             // 00110110: LD (IX+d), n
    &Z80::z80Scf,                     // 00110111: SCF
    &Z80::z80JrCcByte,                // 00111000: JR C, n
    &Z80::z80AddIxRegX,               // 00111001: ADD IX, SP
    &Z80::z80LdAPtrWord,              // 00111010: LD A, (nn)
    &Z80::z80DecRegPX,                // 00111011: DEC SP
    &Z80::z80IncRegX,                 // 00111100: INC A
    &Z80::z80DecRegX,                 // 00111101: DEC A
    &Z80::z80LdRegXByte,              // 00111110: LD A, n
    &Z80::z80Ccf,                     // 00111111: CCF
    &Z80::z80LdRegXRegX,              // 01000000: LD B, B
    &Z80::z80LdRegXRegX,              // 01000001: LD B, C
    &Z80::z80LdRegXRegX,              // 01000010: LD B, D
    &Z80::z80LdRegXRegX,              // 01000011: LD B, E
    &Z80::z80LdRegXRegX,              // 01000100: LD B, IXh
    &Z80::z80LdRegXRegX,              // 01000101: LD B, IXl
    &Z80::z80LdRegPtrIx,              // 01000110: LD B, (IX+d)
    &Z80::z80LdRegXRegX,              // 01000111: LD B, A
    &Z80::z80LdRegXRegX,              // 01001000: LD C, B
    &Z80::z80LdRegXRegX,              // 01001001: LD C, C
    &Z80::z80LdRegXRegX,              // 01001010: LD C, D
    &Z80::z80LdRegXRegX,              // 01001011: LD C, E
    &Z80::z80LdRegXRegX,              // 01001100: LD C, IXh
    &Z80::z80LdRegXRegX,              // 01001101: LD C, IXl
    &Z80::z80LdRegPtrIx,              // 01001110: LD C, (IX+d)
    &Z80::z80LdRegXRegX,              // 01001111: LD C, A
    &Z80::z80LdRegXRegX,              // 01010000: LD D, B
    &Z80::z80LdRegXRegX,              // 01010001: LD D, C
    &Z80::z80LdRegXRegX,              // 01010010: LD D, D
    &Z80::z80LdRegXRegX,              // 01010011: LD D, E
    &Z80::z80LdRegXRegX,              // 01010100: LD D, IXh
    &Z80::z80LdRegXRegX,              // 01010101: LD D, IXl
    &Z80::z80LdRegPtrIx,              // 01010110: LD D, (IX+d)
    &Z80::z80LdRegXRegX,              // 01010111: LD D, A
    &Z80::z80LdRegXRegX,              // 01011000: LD E, B
    &Z80::z80LdRegXRegX,              // 01011001: LD E, C
    &Z80::z80LdRegXRegX,              // 01011010: LD E, D
    &Z80::z80LdRegXRegX,              // 01011011: LD E, E
    &Z80::z80LdRegXRegX,              // 01011100: LD E, IXh
    &Z80::z80LdRegXRegX,              // 01011101: LD E, IXl
    &Z80::z80LdRegPtrIx,              // 01011110: LD E, (IX+d)
    &Z80::z80LdRegXRegX,              // 01011111: LD E, A
    &Z80::z80LdRegXRegX,              // 01100000: LD H, B
    &Z80::z80LdRegXRegX,              // 01100001: LD H, C
    &Z80::z80LdRegXRegX,              // 01100010: LD H, D
    &Z80::z80LdRegXRegX,              // 01100011: LD H, E
    &Z80::z80LdRegXRegX,              // 01100100: LD H, IXh
    &Z80::z80LdRegXRegX,              // 01100101: LD H, IXl
    &Z80::z80LdRegPtrIx,              // 01100110: LD H, (IX+d)
    &Z80::z80LdRegXRegX,              // 01100111: LD H, A
    &Z80::z80LdRegXRegX,              // 01101000: LD L, B
    &Z80::z80LdRegXRegX,              // 01101001: LD L, C
    &Z80::z80LdRegXRegX,              // 01101010: LD L, D
    &Z80::z80LdRegXRegX,              // 01101011: LD L, E
    &Z80::z80LdRegXRegX,              // 01101100: LD L, IXh
    &Z80::z80LdRegXRegX,              // 01101101: LD L, IXl
    &Z80::z80LdRegPtrIx,              // 01101110: LD L, (IX+d)
    &Z80::z80LdRegXRegX,              // 01101111: LD L, A
    &Z80::z80LdPtrIxReg,              // 01110000: LD (IX+d), B
    &Z80::z80LdPtrIxReg,              // 01110001: LD (IX+d), C
    &Z80::z80LdPtrIxReg,              // 01110010: LD (IX+d), D
    &Z80::z80LdPtrIxReg,              // 01110011: LD (IX+d), E
    &Z80::z80LdPtrIxReg,              // 01110100: LD (IX+d), H
    &Z80::z80LdPtrIxReg,              // 01110101: LD (IX+d), L
    &Z80::z80Halt,                    // 01110110: HALT
    &Z80::z80LdPtrIxReg,              // 01110111: LD (IX+d), A
    &Z80::z80LdRegXRegX,              // 01111000: LD A, B
    &Z80::z80LdRegXRegX,              // 01111001: LD A, C
    &Z80::z80LdRegXRegX,              // 01111010: LD A, D
    &Z80::z80LdRegXRegX,              // 01111011: LD A, E
    &Z80::z80LdRegXRegX,              // 01111100: LD A, IXh
    &Z80::z80LdRegXRegX,              // 01111101: LD A, IXl
    &Z80::z80LdRegPtrIx,              // 01111110: LD A, (IX+d)
    &Z80::z80LdRegXRegX,              // 01111111: LD A, A
    &Z80::z80AddRegX,                 // 10000000: ADD A, B
    &Z80::z80AddRegX,                 // 10000001: ADD A, C
    &Z80::z80AddRegX,                 // 10000010: ADD A, D
    &Z80::z80AddRegX,                 // 10000011: ADD A, E         
    &Z80::z80AddRegX,                 // 10000100: ADD A, IXh
    &Z80::z80AddRegX,                 // 10000101: ADD A, IXl
    &Z80::z80AddPtrIx,                // 10000110: ADD A, (IX+d)
    &Z80::z80AddRegX,                 // 10000111: ADD A, A
    &Z80::z80AdcRegX,                 // 10001000: ADC A, B
    &Z80::z80AdcRegX,                 // 10001001: ADC A, C
    &Z80::z80AdcRegX,                 // 10001010: ADC A, D
    &Z80::z80AdcRegX,                 // 10001011: ADC A, E
    &Z80::z80AdcRegX,                 // 10001100: ADC A, IXh
    &Z80::z80AdcRegX,                 // 10001101: ADC A, IXl
    &Z80::z80AdcPtrIx,                // 10001110: ADC A, (IX+d)
    &Z80::z80AdcRegX,                 // 10001111: ADC A, A
    &Z80::z80SubRegX,                 // 10010000: SUB B
    &Z80::z80SubRegX,                 // 10010001: SUB C
    &Z80::z80SubRegX,                 // 10010010: SUB D
    &Z80::z80SubRegX,                 // 10010011: SUB E
    &Z80::z80SubRegX,                 // 10010100: SUB IXh
    &Z80::z80SubRegX,                 // 10010101: SUB IXl
    &Z80::z80SubPtrIx,                // 10010110: SUB (IX+d)
    &Z80::z80SubRegX,                 // 10010111: SUB A
    &Z80::z80SbcRegX,                 // 10011000: SBC A, B
    &Z80::z80SbcRegX,                 // 10011001: SBC A, C
    &Z80::z80SbcRegX,                 // 10011010: SBC A, D
    &Z80::z80SbcRegX,                 // 10011011: SBC A, E
    &Z80::z80SbcRegX,                 // 10011100: SBC A, IXh
    &Z80::z80SbcRegX,                 // 10011101: SBC A, IXl
    &Z80::z80SbcPtrIx,                // 10011110: SBC A, (IX+d)
    &Z80::z80SbcRegX,                 // 10011111: SBC A, A
    &Z80::z80AndRegX,                 // 10100000: AND B
    &Z80::z80AndRegX,                 // 10100001: AND C
    &Z80::z80AndRegX,                 // 10100010: AND D
    &Z80::z80AndRegX,                 // 10100011: AND E
    &Z80::z80AndRegX,                 // 10100100: AND IXh
    &Z80::z80AndRegX,                 // 10100101: AND IXl
    &Z80::z80AndPtrIx,                // 10100110: AND (IX+d)
    &Z80::z80AndRegX,                 // 10100111: AND A
    &Z80::z80XorRegX,                 // 10101000: XOR B
    &Z80::z80XorRegX,                 // 10101001: XOR C
    &Z80::z80XorRegX,                 // 10101010: XOR D
    &Z80::z80XorRegX,                 // 10101011: XOR E
    &Z80::z80XorRegX,                 // 10101100: XOR IXh
    &Z80::z80XorRegX,                 // 10101101: XOR IXl
    &Z80::z80XorPtrIx,                // 10101110: XOR (IX+d)
    &Z80::z80XorRegX,                 // 10101111: XOR A
    &Z80::z80OrRegX,                  // 10110000: OR B
    &Z80::z80OrRegX,                  // 10110001: OR C
    &Z80::z80OrRegX,                  // 10110010: OR D
    &Z80::z80OrRegX,                  // 10110011: OR E
    &Z80::z80OrRegX,                  // 10110100: OR IXh
    &Z80::z80OrRegX,                  // 10110101: OR IXl
    &Z80::z80OrPtrIx,                 // 10110110: OR (IX+d)
    &Z80::z80OrRegX,                  // 10110111: OR A
    &Z80::z80CpRegX,                  // 10111000: CP B
    &Z80::z80CpRegX,                  // 10111001: CP C
    &Z80::z80CpRegX,                  // 10111010: CP D
    &Z80::z80CpRegX,                  // 10111011: CP E
    &Z80::z80CpRegX,                  // 10111100: CP IXh
    &Z80::z80CpRegX,                  // 10111101: CP IXl
    &Z80::z80CpPtrIx,                 // 10111110: CP (IX+d)
    &Z80::z80CpRegX,                  // 10111111: CP A
    &Z80::z80RetCc,                   // 11000000: RET NZ
    &Z80::z80PopReg,                  // 11000001: POP BC
    &Z80::z80JpCcWord,                // 11000010: JP NZ, nn
    &Z80::z80JpWord,                  // 11000011: JP nn
    &Z80::z80CallCc,                  // 11000100: CALL NZ, nn
    &Z80::z80PushReg,                 // 11000101: PUSH BC
    &Z80::z80AddByte,                 // 11000110: ADD A, n
    &Z80::z80Rst,                     // 11000111: RST 0h
    &Z80::z80RetCc,                   // 11001000: RET Z
    &Z80::z80Ret,                     // 11001001: RET
    &Z80::z80JpCcWord,                // 11001010: JP Z, nn
    &Z80::z80PrefixDDCB,              // 11001011: CB Prefix
    &Z80::z80CallCc,                  // 11001100: CALL Z, nn
    &Z80::z80Call,                    // 11001101: CALL nn
    &Z80::z80AdcByte,                 // 11001110: ADC A, n
    &Z80::z80Rst,                     // 11001111: RST 8h
    &Z80::z80RetCc,                   // 11010000: RET NC
    &Z80::z80PopReg,                  // 11010001: POP DE
    &Z80::z80JpCcWord,                // 11010010: JP NC, nn
    &Z80::z80OutPtrByteA,             // 11010011: OUT (n), A
    &Z80::z80CallCc,                  // 11010100: CALL NC, nn
    &Z80::z80PushReg,                 // 11010101: PUSH DE
    &Z80::z80SubByte,                 // 11010110: SUB n
    &Z80::z80Rst,                     // 11010111: RST 10h
    &Z80::z80RetCc,                   // 11011000: RET C
    &Z80::z80Exx,                     // 11011001: EXX
    &Z80::z80JpCcWord,                // 11011010: JP C, nn
    &Z80::z80InAPtrByte,              // 11011011: IN A, (n)
    &Z80::z80CallCc,                  // 11011100: CALL C, nn
    &Z80::z80PrefixDD,                // 11011101: DD Prefix
    &Z80::z80SbcByte,                 // 11011110: SBC A, n
    &Z80::z80Rst,                     // 11011111: RST 18h
    &Z80::z80RetCc,                   // 11100000: RET PO
    &Z80::z80PopIx,                   // 11100001: POP IX
    &Z80::z80JpCcWord,                // 11100010: JP PO, nn
    &Z80::z80ExPtrSpIx,               // 11100011: EX (SP), IX
    &Z80::z80CallCc,                  // 11100100: CALL PO, nn
    &Z80::z80PushIx,                  // 11100101: PUSH IX
    &Z80::z80AndByte,                 // 11100110: AND n
    &Z80::z80Rst,                     // 11100111: RST 20h
    &Z80::z80RetCc,                   // 11101000: RET PE
    &Z80::z80JpIx,                    // 11101001: JP (IX)
    &Z80::z80JpCcWord,                // 11101010: JP PE, nn
    &Z80::z80ExDeHl,                  // 11101011: EX DE, HL
    &Z80::z80CallCc,                  // 11101100: CALL PE, nn
    &Z80::z80PrefixED,                // 11101101: ED Prefix
    &Z80::z80XorByte,                 // 11101110: XOR n
    &Z80::z80Rst,                     // 11101111: RST 28h
    &Z80::z80RetCc,                   // 11110000: RET P
    &Z80::z80PopReg,                  // 11110001: POP AF
    &Z80::z80JpCcWord,                // 11110010: JP P, nn
    &Z80::z80Di,                      // 11110011: DI
    &Z80::z80CallCc,                  // 11110100: CALL P, nn
    &Z80::z80PushReg,                 // 11110101: PUSH AF
    &Z80::z80OrByte,                  // 11110110: OR n
    &Z80::z80Rst,                     // 11110111: RST 30h
    &Z80::z80RetCc,                   // 11111000: RET M
    &Z80::z80LdSpIx,                  // 11111001: LD SP, IX
    &Z80::z80JpCcWord,                // 11111010: JP M, nn
    &Z80::z80Ei,                      // 11111011: EI
    &Z80::z80CallCc,                  // 11111100: CALL M, nn
    &Z80::z80PrefixFD,                // 11111101: FD Prefix
    &Z80::z80CpByte,                  // 11111110: CP n
    &Z80::z80Rst                      // 11111111: RST 38h
},
