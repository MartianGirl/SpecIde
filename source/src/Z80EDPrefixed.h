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

/** Z80EDPrefixed.h
 *
 * Z80 instruction table for ED-prefixed opcodes.
 *
 */

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

#include "Z80Rld.h"
#include "Z80Rrd.h"


/*
bool z80EDPrefixed(uint_fast8_t opcode)
{
    switch (opcode)
    {
        case 0x00: return z80Nop();
        case 0x01: return z80Nop();
        case 0x02: return z80Nop();
        case 0x03: return z80Nop();
        case 0x04: return z80Nop();
        case 0x05: return z80Nop();
        case 0x06: return z80Nop();
        case 0x07: return z80Nop();
        case 0x08: return z80Nop();
        case 0x09: return z80Nop();
        case 0x0A: return z80Nop();
        case 0x0B: return z80Nop();
        case 0x0C: return z80Nop();
        case 0x0D: return z80Nop();
        case 0x0E: return z80Nop();
        case 0x0F: return z80Nop();
        case 0x10: return z80Nop();
        case 0x11: return z80Nop();
        case 0x12: return z80Nop();
        case 0x13: return z80Nop();
        case 0x14: return z80Nop();
        case 0x15: return z80Nop();
        case 0x16: return z80Nop();
        case 0x17: return z80Nop();
        case 0x18: return z80Nop();
        case 0x19: return z80Nop();
        case 0x1A: return z80Nop();
        case 0x1B: return z80Nop();
        case 0x1C: return z80Nop();
        case 0x1D: return z80Nop();
        case 0x1E: return z80Nop();
        case 0x1F: return z80Nop();
        case 0x20: return z80Nop();
        case 0x21: return z80Nop();
        case 0x22: return z80Nop();
        case 0x23: return z80Nop();
        case 0x24: return z80Nop();
        case 0x25: return z80Nop();
        case 0x26: return z80Nop();
        case 0x27: return z80Nop();
        case 0x28: return z80Nop();
        case 0x29: return z80Nop();
        case 0x2A: return z80Nop();
        case 0x2B: return z80Nop();
        case 0x2C: return z80Nop();
        case 0x2D: return z80Nop();
        case 0x2E: return z80Nop();
        case 0x2F: return z80Nop();
        case 0x30: return z80Nop();
        case 0x31: return z80Nop();
        case 0x32: return z80Nop();
        case 0x33: return z80Nop();
        case 0x34: return z80Nop();
        case 0x35: return z80Nop();
        case 0x36: return z80Nop();
        case 0x37: return z80Nop();
        case 0x38: return z80Nop();
        case 0x39: return z80Nop();
        case 0x3A: return z80Nop();
        case 0x3B: return z80Nop();
        case 0x3C: return z80Nop();
        case 0x3D: return z80Nop();
        case 0x3E: return z80Nop();
        case 0x3F: return z80Nop();
        case 0x40: return z80InRegPtrC();       // 01000000: IN B, (C)
        case 0x41: return z80OutPtrCReg();      // 01000001: OUT (C), B
        case 0x42: return z80SbcHlReg();        // 01000010: SBC HL, BC
        case 0x43: return z80LdPtrWordReg();    // 01000011: LD (nn), BC
        case 0x44: return z80Neg();             // 01000100: NEG
        case 0x45: return z80RetI();            // 01000101: RETN
        case 0x46: return z80Im();              // 01000110: IM 0
        case 0x47: return z80LdIA();            // 01000111: LD I, A
        case 0x48: return z80InRegPtrC();       // 01001000: IN C, (C)
        case 0x49: return z80OutPtrCReg();      // 01001001: OUT (C), C
        case 0x4A: return z80AdcHlReg();        // 01001010: ADC HL, BC
        case 0x4B: return z80LdRegPtrWord();    // 01001011: LD BC, (nn)
        case 0x4C: return z80Neg();             // 01001100: NEG*
        case 0x4D: return z80RetI();            // 01001101: RETI
        case 0x4E: return z80Im();              // 01001110: IM 0*
        case 0x4F: return z80LdRA();            // 01001111: LD R, A
        case 0x50: return z80InRegPtrC();       // 01010000: IN D, (C)
        case 0x51: return z80OutPtrCReg();      // 01010001: OUT (C), D
        case 0x52: return z80SbcHlReg();        // 01010010: SBC HL, DE
        case 0x53: return z80LdPtrWordReg();    // 01010011: LD (nn), DE
        case 0x54: return z80Neg();             // 01010100: NEG*
        case 0x55: return z80RetI();            // 01010101: RETN*
        case 0x56: return z80Im();              // 01010110: IM 1
        case 0x57: return z80LdAI();            // 01010111: LD A, I
        case 0x58: return z80InRegPtrC();       // 01011000: IN E, (C)
        case 0x59: return z80OutPtrCReg();      // 01011001: OUT (C), E
        case 0x5A: return z80AdcHlReg();        // 01011010: ADC HL, DE
        case 0x5B: return z80LdRegPtrWord();    // 01011011: LD DE, (nn)
        case 0x5C: return z80Neg();             // 01011100: NEG*
        case 0x5D: return z80RetI();            // 01011101: RETN*
        case 0x5E: return z80Im();              // 01011110: IM 2
        case 0x5F: return z80LdAR();            // 01011111: LD A, R
        case 0x60: return z80InRegPtrC();       // 01100000: IN H, (C)
        case 0x61: return z80OutPtrCReg();      // 01100001: OUT (C), H
        case 0x62: return z80SbcHlReg();        // 01100010: SBC HL, HL
        case 0x63: return z80LdPtrWordReg();    // 01100011: LD (nn), HL
        case 0x64: return z80Neg();             // 01100100: NEG*
        case 0x65: return z80RetI();            // 01100101: RETN*
        case 0x66: return z80Im();              // 01100110: IM 0*
        case 0x67: return z80Rrd();             // 01100111: RRD
        case 0x68: return z80InRegPtrC();       // 01101000: IN L, (C)
        case 0x69: return z80OutPtrCReg();      // 01101001: OUT (C), L
        case 0x6A: return z80AdcHlReg();        // 01101010: ADC HL, HL
        case 0x6B: return z80LdRegPtrWord();    // 01101011: LD HL, (nn)
        case 0x6C: return z80Neg();             // 01101100: NEG
        case 0x6D: return z80RetI();            // 01101101: RETN*
        case 0x6E: return z80Im();              // 01101110: IM 0*
        case 0x6F: return z80Rld();             // 01101111: RLD
        case 0x70: return z80InRegPtrC();       // 01110000: IN F, (C)
        case 0x71: return z80OutPtrCReg();      // 01110001: OUT (C), 0
        case 0x72: return z80SbcHlReg();        // 01110010: SBC HL, SP
        case 0x73: return z80LdPtrWordReg();    // 01110011: LD (nn), SP
        case 0x74: return z80Neg();             // 01110100: NEG
        case 0x75: return z80RetI();            // 01110101: RETN*
        case 0x76: return z80Im();              // 01110110: IM 1*
        case 0x77: return z80Nop();             // 01110111: NOP*
        case 0x78: return z80InRegPtrC();       // 01111000: IN A, (C)
        case 0x79: return z80OutPtrCReg();      // 01111001: OUT (C), A
        case 0x7A: return z80AdcHlReg();        // 01111010: ADC HL, SP
        case 0x7B: return z80LdRegPtrWord();    // 01111011: LD SP, (nn)
        case 0x7C: return z80Neg();             // 01111100: NEG*
        case 0x7D: return z80RetI();            // 01111101: RETN*
        case 0x7E: return z80Im();              // 01111110: IM 2*
        case 0x7F: return z80Nop();             // 01111111: NOP*
        case 0x80: return z80Nop();
        case 0x81: return z80Nop();
        case 0x82: return z80Nop();
        case 0x83: return z80Nop();
        case 0x84: return z80Nop();
        case 0x85: return z80Nop();
        case 0x86: return z80Nop();
        case 0x87: return z80Nop();
        case 0x88: return z80Nop();
        case 0x89: return z80Nop();
        case 0x8A: return z80Nop();
        case 0x8B: return z80Nop();
        case 0x8C: return z80Nop();
        case 0x8D: return z80Nop();
        case 0x8E: return z80Nop();
        case 0x8F: return z80Nop();
        case 0x90: return z80Nop();
        case 0x91: return z80Nop();
        case 0x92: return z80Nop();
        case 0x93: return z80Nop();
        case 0x94: return z80Nop();
        case 0x95: return z80Nop();
        case 0x96: return z80Nop();
        case 0x97: return z80Nop();
        case 0x98: return z80Nop();
        case 0x99: return z80Nop();
        case 0x9A: return z80Nop();
        case 0x9B: return z80Nop();
        case 0x9C: return z80Nop();
        case 0x9D: return z80Nop();
        case 0x9E: return z80Nop();
        case 0x9F: return z80Nop();
        case 0xA0: return z80Ldi();         // 10100000: LDI
        case 0xA1: return z80Cpi();         // 10100001: CPI
        case 0xA2: return z80Ini();         // 10100010: INI
        case 0xA3: return z80Outi();        // 10100011: OUTI
        case 0xA4: return z80Nop();
        case 0xA5: return z80Nop();
        case 0xA6: return z80Nop();
        case 0xA7: return z80Nop();
        case 0xA8: return z80Ldd();         // 10101000: LDD
        case 0xA9: return z80Cpd();         // 10101001: CPD
        case 0xAA: return z80Ind();         // 10101010: IND
        case 0xAB: return z80Outd();        // 10101011: OUTD
        case 0xAC: return z80Nop();
        case 0xAD: return z80Nop();
        case 0xAE: return z80Nop();
        case 0xAF: return z80Nop();
        case 0xB0: return z80Ldir();        // 10110000: LDIR
        case 0xB1: return z80Cpir();        // 10110001: CPIR
        case 0xB2: return z80Inir();        // 10110010: INIR
        case 0xB3: return z80Otir();        // 10110011: OTIR
        case 0xB4: return z80Nop();
        case 0xB5: return z80Nop();
        case 0xB6: return z80Nop();
        case 0xB7: return z80Nop();
        case 0xB8: return z80Lddr();        // 10111000: LDDR
        case 0xB9: return z80Cpdr();        // 10111001: CPDR
        case 0xBA: return z80Indr();        // 10111010: INDR
        case 0xBB: return z80Otdr();        // 10111011: OTDR
        case 0xBC: return z80Nop();
        case 0xBD: return z80Nop();
        case 0xBE: return z80Nop();
        case 0xBF: return z80Nop();
        case 0xC0: return z80Nop();
        case 0xC1: return z80Nop();
        case 0xC2: return z80Nop();
        case 0xC3: return z80Nop();
        case 0xC4: return z80Nop();
        case 0xC5: return z80Nop();
        case 0xC6: return z80Nop();
        case 0xC7: return z80Nop();
        case 0xC8: return z80Nop();
        case 0xC9: return z80Nop();
        case 0xCA: return z80Nop();
        case 0xCB: return z80Nop();
        case 0xCC: return z80Nop();
        case 0xCD: return z80Nop();
        case 0xCE: return z80Nop();
        case 0xCF: return z80Nop();
        case 0xD0: return z80Nop();
        case 0xD1: return z80Nop();
        case 0xD2: return z80Nop();
        case 0xD3: return z80Nop();
        case 0xD4: return z80Nop();
        case 0xD5: return z80Nop();
        case 0xD6: return z80Nop();
        case 0xD7: return z80Nop();
        case 0xD8: return z80Nop();
        case 0xD9: return z80Nop();
        case 0xDA: return z80Nop();
        case 0xDB: return z80Nop();
        case 0xDC: return z80Nop();
        case 0xDD: return z80Nop();
        case 0xDE: return z80Nop();
        case 0xDF: return z80Nop();
        case 0xE0: return z80Nop();
        case 0xE1: return z80Nop();
        case 0xE2: return z80Nop();
        case 0xE3: return z80Nop();
        case 0xE4: return z80Nop();
        case 0xE5: return z80Nop();
        case 0xE6: return z80Nop();
        case 0xE7: return z80Nop();
        case 0xE8: return z80Nop();
        case 0xE9: return z80Nop();
        case 0xEA: return z80Nop();
        case 0xEB: return z80Nop();
        case 0xEC: return z80Nop();
        case 0xED: return z80Nop();
        case 0xEE: return z80Nop();
        case 0xEF: return z80Nop();
        case 0xF0: return z80Nop();
        case 0xF1: return z80Nop();
        case 0xF2: return z80Nop();
        case 0xF3: return z80Nop();
        case 0xF4: return z80Nop();
        case 0xF5: return z80Nop();
        case 0xF6: return z80Nop();
        case 0xF7: return z80Nop();
        case 0xF8: return z80Nop();
        case 0xF9: return z80Nop();
        case 0xFA: return z80Nop();
        case 0xFB: return z80Nop();
        case 0xFC: return z80Nop();
        case 0xFD: return z80Nop();
        case 0xFE: return z80Nop();
        case 0xFF: return z80Nop();
        default: assert(false); return true;   // Should not happen
    }
}
*/

// vim: et:sw=4:ts=4
