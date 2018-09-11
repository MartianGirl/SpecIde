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

#pragma once

/** Z80FDPrefixed.h
 *
 * Z80 instruction table for FD-prefixed opcodes.
 *
 */

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

#include "Z80Rlca.h"
#include "Z80Rla.h"
#include "Z80Rrca.h"
#include "Z80Rra.h"

#include "Z80PrefixFDCB.h"
#include "Z80PrefixDD.h"
#include "Z80PrefixED.h"
#include "Z80PrefixFD.h"

/*
bool z80FDPrefixed(uint_fast8_t opcode)
{
    switch (opcode)
    {
        case 0x00: return z80Nop();             // 00000000: NOP
        case 0x01: return z80LdRegYWord();      // 00000001: LD BC, nn
        case 0x02: return z80LdPtrBcA();        // 00000010: LD (BC), A
        case 0x03: return z80IncRegPY();        // 00000011: INC BC
        case 0x04: return z80IncRegY();         // 00000100: INC B
        case 0x05: return z80DecRegY();         // 00000101: DEC B
        case 0x06: return z80LdRegYByte();      // 00000110: LD B, n
        case 0x07: return z80Rlca();            // 00000111: RLCA
        case 0x08: return z80ExAfAf();          // 00001000: EX AF, AF'
        case 0x09: return z80AddIyRegY();       // 00001001: ADD IY, BC
        case 0x0A: return z80LdAPtrBc();        // 00001010: LD A, (BC)
        case 0x0B: return z80DecRegPY();        // 00001011: DEC BC
        case 0x0C: return z80IncRegY();         // 00001100: INC C
        case 0x0D: return z80DecRegY();         // 00001101: DEC C
        case 0x0E: return z80LdRegYByte();      // 00001110: LD C, n
        case 0x0F: return z80Rrca();            // 00001111: RRCA
        case 0x10: return z80Djnz();            // 00010000: DJNZ n
        case 0x11: return z80LdRegYWord();      // 00010001: LD DE, nn
        case 0x12: return z80LdPtrDeA();        // 00010010: LD (DE), A
        case 0x13: return z80IncRegPY();        // 00010011: INC DE
        case 0x14: return z80IncRegY();         // 00010100: INC D
        case 0x15: return z80DecRegY();         // 00010101: DEC D
        case 0x16: return z80LdRegYByte();      // 00010110: LD D, n
        case 0x17: return z80Rla();             // 00010111: RLA
        case 0x18: return z80JrByte();          // 00011000: JR n
        case 0x19: return z80AddIyRegY();       // 00011001: ADD IY, DE
        case 0x1A: return z80LdAPtrDe();        // 00011010: LD A, (DE)
        case 0x1B: return z80DecRegPY();        // 00011011: DEC DE
        case 0x1C: return z80IncRegY();         // 00011100: INC E
        case 0x1D: return z80DecRegY();         // 00011100: DEC E
        case 0x1E: return z80LdRegYByte();      // 00011110: LD E, n
        case 0x1F: return z80Rra();             // 00011111: RRA
        case 0x20: return z80JrCcByte();        // 00100000: JR NZ, n
        case 0x21: return z80LdRegYWord();      // 00100001: LD IY, nn
        case 0x22: return z80LdPtrWordIy();     // 00100010: LD (nn), IY
        case 0x23: return z80IncRegPY();        // 00100011: INC IY
        case 0x24: return z80IncRegY();         // 00100100: INC IYh
        case 0x25: return z80DecRegY();         // 00100101: DEC IYh
        case 0x26: return z80LdRegYByte();      // 00100110: LD IYh, n
        case 0x27: return z80Daa();             // 00100111: DAA
        case 0x28: return z80JrCcByte();        // 00101000: JR Z, n
        case 0x29: return z80AddIyRegY();       // 00101001: ADD IY, IY
        case 0x2A: return z80LdIyPtrWord();     // 00101010: LD IY, (nn)
        case 0x2B: return z80DecRegPY();        // 00101011: DEC IY
        case 0x2C: return z80IncRegY();         // 00101100: INC IYl
        case 0x2D: return z80DecRegY();         // 00101101: DEC IYl
        case 0x2E: return z80LdRegYByte();      // 00101110: LD IYl, n
        case 0x2F: return z80Cpl();             // 00101111: CPL
        case 0x30: return z80JrCcByte();        // 00110000: JR NC, n
        case 0x31: return z80LdRegYWord();      // 00110001: LD SP, nn
        case 0x32: return z80LdPtrWordA();      // 00110010: LD (nn), A
        case 0x33: return z80IncRegPY();        // 00110011: INC SP
        case 0x34: return z80IncPtrIy();        // 00110100: INC (IY+d)
        case 0x35: return z80DecPtrIy();        // 00110101: DEC (IY+d)
        case 0x36: return z80LdPtrIyByte();     // 00110110: LD (IY+d), n
        case 0x37: return z80Scf();             // 00110111: SCF
        case 0x38: return z80JrCcByte();        // 00111000: JR C, n
        case 0x39: return z80AddIyRegY();       // 00111001: ADD IY, SP
        case 0x3A: return z80LdAPtrWord();      // 00111010: LD A, (nn)
        case 0x3B: return z80DecRegPY();        // 00111011: DEC SP
        case 0x3C: return z80IncRegY();         // 00111100: INC A
        case 0x3D: return z80DecRegY();         // 00111101: DEC A
        case 0x3E: return z80LdRegYByte();      // 00111110: LD A, n
        case 0x3F: return z80Ccf();             // 00111111: CCF
        case 0x40: return z80LdRegYRegY();      // 01000000: LD B, B
        case 0x41: return z80LdRegYRegY();      // 01000001: LD B, C
        case 0x42: return z80LdRegYRegY();      // 01000010: LD B, D
        case 0x43: return z80LdRegYRegY();      // 01000011: LD B, E
        case 0x44: return z80LdRegYRegY();      // 01000100: LD B, IYh
        case 0x45: return z80LdRegYRegY();      // 01000101: LD B, IYl
        case 0x46: return z80LdRegPtrIy();      // 01000110: LD B, (IY+d)
        case 0x47: return z80LdRegYRegY();      // 01000111: LD B, A
        case 0x48: return z80LdRegYRegY();      // 01001000: LD C, B
        case 0x49: return z80LdRegYRegY();      // 01001001: LD C, C
        case 0x4A: return z80LdRegYRegY();      // 01001010: LD C, D
        case 0x4B: return z80LdRegYRegY();      // 01001011: LD C, E
        case 0x4C: return z80LdRegYRegY();      // 01001100: LD C, IYh
        case 0x4D: return z80LdRegYRegY();      // 01001101: LD C, IYl
        case 0x4E: return z80LdRegPtrIy();      // 01001110: LD C, (IY+d)
        case 0x4F: return z80LdRegYRegY();      // 01001111: LD C, A
        case 0x50: return z80LdRegYRegY();      // 01010000: LD D, B
        case 0x51: return z80LdRegYRegY();      // 01010001: LD D, C
        case 0x52: return z80LdRegYRegY();      // 01010010: LD D, D
        case 0x53: return z80LdRegYRegY();      // 01010011: LD D, E
        case 0x54: return z80LdRegYRegY();      // 01010100: LD D, IYh
        case 0x55: return z80LdRegYRegY();      // 01010101: LD D, IYl
        case 0x56: return z80LdRegPtrIy();      // 01010110: LD D, (IY+d)
        case 0x57: return z80LdRegYRegY();      // 01010111: LD D, A
        case 0x58: return z80LdRegYRegY();      // 01011000: LD E, B
        case 0x59: return z80LdRegYRegY();      // 01011001: LD E, C
        case 0x5A: return z80LdRegYRegY();      // 01011010: LD E, D
        case 0x5B: return z80LdRegYRegY();      // 01011011: LD E, E
        case 0x5C: return z80LdRegYRegY();      // 01011100: LD E, IYh
        case 0x5D: return z80LdRegYRegY();      // 01011101: LD E, IYl
        case 0x5E: return z80LdRegPtrIy();      // 01011110: LD E, (IY+d)
        case 0x5F: return z80LdRegYRegY();      // 01011111: LD E, A
        case 0x60: return z80LdRegYRegY();      // 01100000: LD H, B
        case 0x61: return z80LdRegYRegY();      // 01100001: LD H, C
        case 0x62: return z80LdRegYRegY();      // 01100010: LD H, D
        case 0x63: return z80LdRegYRegY();      // 01100011: LD H, E
        case 0x64: return z80LdRegYRegY();      // 01100100: LD H, IYh
        case 0x65: return z80LdRegYRegY();      // 01100101: LD H, IYl
        case 0x66: return z80LdRegPtrIy();      // 01100110: LD H, (IY+d)
        case 0x67: return z80LdRegYRegY();      // 01100111: LD H, A
        case 0x68: return z80LdRegYRegY();      // 01101000: LD L, B
        case 0x69: return z80LdRegYRegY();      // 01101001: LD L, C
        case 0x6A: return z80LdRegYRegY();      // 01101010: LD L, D
        case 0x6B: return z80LdRegYRegY();      // 01101011: LD L, E
        case 0x6C: return z80LdRegYRegY();      // 01101100: LD L, IYh
        case 0x6D: return z80LdRegYRegY();      // 01101101: LD L, IYl
        case 0x6E: return z80LdRegPtrIy();      // 01101110: LD L, (IY+d)
        case 0x6F: return z80LdRegYRegY();      // 01101111: LD L, A
        case 0x70: return z80LdPtrIyReg();      // 01110000: LD (IY+d), B
        case 0x71: return z80LdPtrIyReg();      // 01110001: LD (IY+d), C
        case 0x72: return z80LdPtrIyReg();      // 01110010: LD (IY+d), D
        case 0x73: return z80LdPtrIyReg();      // 01110011: LD (IY+d), E
        case 0x74: return z80LdPtrIyReg();      // 01110100: LD (IY+d), H
        case 0x75: return z80LdPtrIyReg();      // 01110101: LD (IY+d), L
        case 0x76: return z80Halt();            // 01110110: HALT
        case 0x77: return z80LdPtrIyReg();      // 01110111: LD (IY+d), A
        case 0x78: return z80LdRegYRegY();      // 01111000: LD A, B
        case 0x79: return z80LdRegYRegY();      // 01111001: LD A, C
        case 0x7A: return z80LdRegYRegY();      // 01111010: LD A, D
        case 0x7B: return z80LdRegYRegY();      // 01111011: LD A, E
        case 0x7C: return z80LdRegYRegY();      // 01111100: LD A, IYh
        case 0x7D: return z80LdRegYRegY();      // 01111101: LD A, IYl
        case 0x7E: return z80LdRegPtrIy();      // 01111110: LD A, (IY+d)
        case 0x7F: return z80LdRegYRegY();      // 01111111: LD A, A
        case 0x80: return z80AddRegY();         // 10000000: ADD A, B
        case 0x81: return z80AddRegY();         // 10000001: ADD A, C
        case 0x82: return z80AddRegY();         // 10000010: ADD A, D
        case 0x83: return z80AddRegY();         // 10000011: ADD A, E
        case 0x84: return z80AddRegY();         // 10000100: ADD A, IYh
        case 0x85: return z80AddRegY();         // 10000101: ADD A, IYl
        case 0x86: return z80AddPtrIy();        // 10000110: ADD A, (IY+d)
        case 0x87: return z80AddRegY();         // 10000111: ADD A, A
        case 0x88: return z80AdcRegY();         // 10001000: ADC A, B
        case 0x89: return z80AdcRegY();         // 10001001: ADC A, C
        case 0x8A: return z80AdcRegY();         // 10001010: ADC A, D
        case 0x8B: return z80AdcRegY();         // 10001011: ADC A, E
        case 0x8C: return z80AdcRegY();         // 10001100: ADC A, IYh
        case 0x8D: return z80AdcRegY();         // 10001101: ADC A, IYl
        case 0x8E: return z80AdcPtrIy();        // 10001110: ADC A, (IY+d)
        case 0x8F: return z80AdcRegY();         // 10001111: ADC A, A
        case 0x90: return z80SubRegY();         // 10010000: SUB B
        case 0x91: return z80SubRegY();         // 10010001: SUB C
        case 0x92: return z80SubRegY();         // 10010010: SUB D
        case 0x93: return z80SubRegY();         // 10010011: SUB E
        case 0x94: return z80SubRegY();         // 10010100: SUB IYh
        case 0x95: return z80SubRegY();         // 10010101: SUB IYl
        case 0x96: return z80SubPtrIy();        // 10010110: SUB (IY+d)
        case 0x97: return z80SubRegY();         // 10010111: SUB A
        case 0x98: return z80SbcRegY();         // 10011000: SBC A, B
        case 0x99: return z80SbcRegY();         // 10011001: SBC A, C
        case 0x9A: return z80SbcRegY();         // 10011010: SBC A, D
        case 0x9B: return z80SbcRegY();         // 10011011: SBC A, E
        case 0x9C: return z80SbcRegY();         // 10011100: SBC A, IYh
        case 0x9D: return z80SbcRegY();         // 10011101: SBC A, IYl
        case 0x9E: return z80SbcPtrIy();        // 10011110: SBC A, (IY+d)
        case 0x9F: return z80SbcRegY();         // 10011111: SBC A, A
        case 0xA0: return z80AndRegY();         // 10100000: AND B
        case 0xA1: return z80AndRegY();         // 10100001: AND C
        case 0xA2: return z80AndRegY();         // 10100010: AND D
        case 0xA3: return z80AndRegY();         // 10100011: AND E
        case 0xA4: return z80AndRegY();         // 10100100: AND IYh
        case 0xA5: return z80AndRegY();         // 10100101: AND IYl
        case 0xA6: return z80AndPtrIy();        // 10100110: AND (IY+d)
        case 0xA7: return z80AndRegY();         // 10100111: AND A
        case 0xA8: return z80XorRegY();         // 10101000: XOR B
        case 0xA9: return z80XorRegY();         // 10101001: XOR C
        case 0xAA: return z80XorRegY();         // 10101010: XOR D
        case 0xAB: return z80XorRegY();         // 10101011: XOR E
        case 0xAC: return z80XorRegY();         // 10101100: XOR IYh
        case 0xAD: return z80XorRegY();         // 10101101: XOR IYl
        case 0xAE: return z80XorPtrIy();        // 10101110: XOR (IY+d)
        case 0xAF: return z80XorRegY();         // 10101111: XOR A
        case 0xB0: return z80OrRegY();          // 10110000: OR B
        case 0xB1: return z80OrRegY();          // 10110001: OR C
        case 0xB2: return z80OrRegY();          // 10110010: OR D
        case 0xB3: return z80OrRegY();          // 10110011: OR E
        case 0xB4: return z80OrRegY();          // 10110100: OR IYh
        case 0xB5: return z80OrRegY();          // 10110101: OR IYl
        case 0xB6: return z80OrPtrIy();         // 10110110: OR (IY+d)
        case 0xB7: return z80OrRegY();          // 10110111: OR A
        case 0xB8: return z80CpRegY();          // 10111000: CP B
        case 0xB9: return z80CpRegY();          // 10111001: CP C
        case 0xBA: return z80CpRegY();          // 10111010: CP D
        case 0xBB: return z80CpRegY();          // 10111011: CP E
        case 0xBC: return z80CpRegY();          // 10111100: CP IYh
        case 0xBD: return z80CpRegY();          // 10111101: CP IYl
        case 0xBE: return z80CpPtrIy();         // 10111110: CP (IY+d)
        case 0xBF: return z80CpRegY();          // 10111111: CP A
        case 0xC0: return z80RetCc();           // 11000000: RET NZ
        case 0xC1: return z80PopReg();          // 11000001: POP BC
        case 0xC2: return z80JpCcWord();        // 11000010: JP NZ, nn
        case 0xC3: return z80JpWord();          // 11000011: JP nn
        case 0xC4: return z80CallCc();          // 11000100: CALL NZ, nn
        case 0xC5: return z80PushReg();         // 11000101: PUSH BC
        case 0xC6: return z80AddByte();         // 11000110: ADD A, n
        case 0xC7: return z80Rst();             // 11000111: RST 0h
        case 0xC8: return z80RetCc();           // 11001000: RET Z
        case 0xC9: return z80Ret();             // 11001001: RET
        case 0xCA: return z80JpCcWord();        // 11001010: JP Z, nn
        case 0xCB: return z80PrefixFDCB();      // 11001011: CB Prefix
        case 0xCC: return z80CallCc();          // 11001100: CALL Z, nn
        case 0xCD: return z80Call();            // 11001101: CALL nn
        case 0xCE: return z80AdcByte();         // 11001110: ADC A, n
        case 0xCF: return z80Rst();             // 11001111: RST 8h
        case 0xD0: return z80RetCc();           // 11010000: RET NC
        case 0xD1: return z80PopReg();          // 11010001: POP DE
        case 0xD2: return z80JpCcWord();        // 11010010: JP NC, nn
        case 0xD3: return z80OutPtrByteA();     // 11010011: OUT (n), A
        case 0xD4: return z80CallCc();          // 11010100: CALL NC, nn
        case 0xD5: return z80PushReg();         // 11010101: PUSH DE
        case 0xD6: return z80SubByte();         // 11010110: SUB n
        case 0xD7: return z80Rst();             // 11010111: RST 10h
        case 0xD8: return z80RetCc();           // 11011000: RET C
        case 0xD9: return z80Exx();             // 11011001: EXX
        case 0xDA: return z80JpCcWord();        // 11011010: JP C, nn
        case 0xDB: return z80InAPtrByte();      // 11011011: IN A, (n)
        case 0xDC: return z80CallCc();          // 11011100: CALL C, nn
        case 0xDD: return z80PrefixDD();        // 11011101: DD Prefix
        case 0xDE: return z80SbcByte();         // 11011110: SBC A, n
        case 0xDF: return z80Rst();             // 11011111: RST 18h
        case 0xE0: return z80RetCc();           // 11100000: RET PO
        case 0xE1: return z80PopIy();           // 11100001: POP IY
        case 0xE2: return z80JpCcWord();        // 11100010: JP PO, nn
        case 0xE3: return z80ExPtrSpIy();       // 11100011: EX (SP), IY
        case 0xE4: return z80CallCc();          // 11100100: CALL PO, nn
        case 0xE5: return z80PushIy();          // 11100101: PUSH IY
        case 0xE6: return z80AndByte();         // 11100110: AND n
        case 0xE7: return z80Rst();             // 11100111: RST 20h
        case 0xE8: return z80RetCc();           // 11101000: RET PE
        case 0xE9: return z80JpIy();            // 11101001: JP (IY)
        case 0xEA: return z80JpCcWord();        // 11101010: JP PE, nn
        case 0xEB: return z80ExDeHl();          // 11101011: EX DE, HL
        case 0xEC: return z80CallCc();          // 11101100: CALL PE, nn
        case 0xED: return z80PrefixED();        // 11101101: ED Prefix
        case 0xEE: return z80XorByte();         // 11101110: XOR n
        case 0xEF: return z80Rst();             // 11101111: RST 28h
        case 0xF0: return z80RetCc();           // 11110000: RET P
        case 0xF1: return z80PopReg();          // 11110001: POP AF
        case 0xF2: return z80JpCcWord();        // 11110010: JP P, nn
        case 0xF3: return z80Di();              // 11110011: DI
        case 0xF4: return z80CallCc();          // 11110100: CALL P, nn
        case 0xF5: return z80PushReg();         // 11110101: PUSH AF
        case 0xF6: return z80OrByte();          // 11110110: OR n
        case 0xF7: return z80Rst();             // 11110111: RST 30h
        case 0xF8: return z80RetCc();           // 11111000: RET M
        case 0xF9: return z80LdSpIy();          // 11111001: LD SP, IY
        case 0xFA: return z80JpCcWord();        // 11111010: JP M, nn
        case 0xFB: return z80Ei();              // 11111011: EI
        case 0xFC: return z80CallCc();          // 11111100: CALL M, nn
        case 0xFD: return z80PrefixFD();        // 11111101: FD Prefix
        case 0xFE: return z80CpByte();          // 11111110: CP n
        case 0xFF: return z80Rst();             // 11111111: RST 38h
        default: assert(false); return true;   // Should not happen
    }
}
*/

// vim: et:sw=4:ts=4
