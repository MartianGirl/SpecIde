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

/** Z80XXCBPrefixed.h
 *
 * Z80 instruction table for DDCB and FDCB-prefixed opcodes.
 * The type of addressing is determined by the prefix.
 *
 */

#include "Z80Nop.h"

#include "Z80RlcPtrIxIy.h"
#include "Z80RrcPtrIxIy.h"
#include "Z80RlPtrIxIy.h"
#include "Z80RrPtrIxIy.h"
#include "Z80SlaPtrIxIy.h"
#include "Z80SraPtrIxIy.h"
#include "Z80SllPtrIxIy.h"
#include "Z80SrlPtrIxIy.h"

#include "Z80BitNPtrIxIy.h"
#include "Z80ResNPtrIxIy.h"
#include "Z80SetNPtrIxIy.h"

/*
bool z80XXCBPrefixed(uint_fast8_t opcode)
{
    switch (opcode)
    {
        case 0x00: return z80RlcPtrIxIy();      // 00000000: RLC (In + d), B
        case 0x01: return z80RlcPtrIxIy();      // 00000001: RLC (In + d), C
        case 0x02: return z80RlcPtrIxIy();      // 00000010: RLC (In + d), D
        case 0x03: return z80RlcPtrIxIy();      // 00000011: RLC (In + d), E
        case 0x04: return z80RlcPtrIxIy();      // 00000100: RLC (In + d), H
        case 0x05: return z80RlcPtrIxIy();      // 00000101: RLC (In + d), L
        case 0x06: return z80RlcPtrIxIy();      // 00000110: RLC (In + d)
        case 0x07: return z80RlcPtrIxIy();      // 00000111: RLC (In + d), A
        case 0x08: return z80RrcPtrIxIy();      // 00001000: RRC (In + d), B
        case 0x09: return z80RrcPtrIxIy();      // 00001001: RRC (In + d), C
        case 0x0A: return z80RrcPtrIxIy();      // 00001010: RRC (In + d), D
        case 0x0B: return z80RrcPtrIxIy();      // 00001011: RRC (In + d), E
        case 0x0C: return z80RrcPtrIxIy();      // 00001100: RRC (In + d), H
        case 0x0D: return z80RrcPtrIxIy();      // 00001101: RRC (In + d), L
        case 0x0E: return z80RrcPtrIxIy();      // 00001110: RRC (In + d)
        case 0x0F: return z80RrcPtrIxIy();      // 00001111: RRC (In + d), A
        case 0x10: return z80RlPtrIxIy();       // 00010000: RL (In + d), B
        case 0x11: return z80RlPtrIxIy();       // 00010001: RL (In + d), C
        case 0x12: return z80RlPtrIxIy();       // 00010010: RL (In + d), D
        case 0x13: return z80RlPtrIxIy();       // 00010011: RL (In + d), E
        case 0x14: return z80RlPtrIxIy();       // 00010100: RL (In + d), H
        case 0x15: return z80RlPtrIxIy();       // 00010101: RL (In + d), L
        case 0x16: return z80RlPtrIxIy();       // 00010110: RL (In + d)
        case 0x17: return z80RlPtrIxIy();       // 00010111: RL (In + d), A
        case 0x18: return z80RrPtrIxIy();       // 00011000: RR (In + d), B
        case 0x19: return z80RrPtrIxIy();       // 00011001: RR (In + d), C
        case 0x1A: return z80RrPtrIxIy();       // 00011010: RR (In + d), D
        case 0x1B: return z80RrPtrIxIy();       // 00011011: RR (In + d), E
        case 0x1C: return z80RrPtrIxIy();       // 00011100: RR (In + d), H
        case 0x1D: return z80RrPtrIxIy();       // 00011101: RR (In + d), L
        case 0x1E: return z80RrPtrIxIy();       // 00011110: RR (In + d)
        case 0x1F: return z80RrPtrIxIy();       // 00011111: RR (In + d), A
        case 0x20: return z80SlaPtrIxIy();      // 00100000: SLA (In + d), B
        case 0x21: return z80SlaPtrIxIy();      // 00100001: SLA (In + d), C
        case 0x22: return z80SlaPtrIxIy();      // 00100010: SLA (In + d), D
        case 0x23: return z80SlaPtrIxIy();      // 00100011: SLA (In + d), E
        case 0x24: return z80SlaPtrIxIy();      // 00100100: SLA (In + d), H
        case 0x25: return z80SlaPtrIxIy();      // 00100101: SLA (In + d), L
        case 0x26: return z80SlaPtrIxIy();      // 00100110: SLA (In + d)
        case 0x27: return z80SlaPtrIxIy();      // 00100111: SLA (In + d), A
        case 0x28: return z80SraPtrIxIy();      // 00101000: SRA (In + d), B
        case 0x29: return z80SraPtrIxIy();      // 00101001: SRA (In + d), C
        case 0x2A: return z80SraPtrIxIy();      // 00101010: SRA (In + d), D
        case 0x2B: return z80SraPtrIxIy();      // 00101011: SRA (In + d), E
        case 0x2C: return z80SraPtrIxIy();      // 00101100: SRA (In + d), H
        case 0x2D: return z80SraPtrIxIy();      // 00101101: SRA (In + d), L
        case 0x2E: return z80SraPtrIxIy();      // 00101110: SRA (In + d)
        case 0x2F: return z80SraPtrIxIy();      // 00101111: SRA (In + d), A
        case 0x30: return z80SllPtrIxIy();      // 00110000: SLL (In + d), B
        case 0x31: return z80SllPtrIxIy();      // 00110001: SLL (In + d), C
        case 0x32: return z80SllPtrIxIy();      // 00110010: SLL (In + d), D
        case 0x33: return z80SllPtrIxIy();      // 00110011: SLL (In + d), E
        case 0x34: return z80SllPtrIxIy();      // 00110100: SLL (In + d), H
        case 0x35: return z80SllPtrIxIy();      // 00110101: SLL (In + d), L
        case 0x36: return z80SllPtrIxIy();      // 00110110: SLL (In + d)
        case 0x37: return z80SllPtrIxIy();      // 00110111: SLL (In + d), A
        case 0x38: return z80SrlPtrIxIy();      // 00111000: SRL (In + d), B
        case 0x39: return z80SrlPtrIxIy();      // 00111001: SRL (In + d), C
        case 0x3A: return z80SrlPtrIxIy();      // 00111010: SRL (In + d), D
        case 0x3B: return z80SrlPtrIxIy();      // 00111011: SRL (In + d), E
        case 0x3C: return z80SrlPtrIxIy();      // 00111100: SRL (In + d), H
        case 0x3D: return z80SrlPtrIxIy();      // 00111101: SRL (In + d), L
        case 0x3E: return z80SrlPtrIxIy();      // 00111110: SRL (In + d)
        case 0x3F: return z80SrlPtrIxIy();      // 00111111: SRL (In + d), A
        case 0x40: return z80BitNPtrIxIy();     // 01000000: BIT 0, (In + d)
        case 0x41: return z80BitNPtrIxIy();     // 01000001: BIT 0, (In + d)
        case 0x42: return z80BitNPtrIxIy();     // 01000010: BIT 0, (In + d)
        case 0x43: return z80BitNPtrIxIy();     // 01000011: BIT 0, (In + d)
        case 0x44: return z80BitNPtrIxIy();     // 01000100: BIT 0, (In + d)
        case 0x45: return z80BitNPtrIxIy();     // 01000101: BIT 0, (In + d)
        case 0x46: return z80BitNPtrIxIy();     // 01000110: BIT 0, (In + d)
        case 0x47: return z80BitNPtrIxIy();     // 01000111: BIT 0, (In + d)
        case 0x48: return z80BitNPtrIxIy();     // 01001000: BIT 1, (In + d)
        case 0x49: return z80BitNPtrIxIy();     // 01001001: BIT 1, (In + d)
        case 0x4A: return z80BitNPtrIxIy();     // 01001010: BIT 1, (In + d)
        case 0x4B: return z80BitNPtrIxIy();     // 01001011: BIT 1, (In + d)
        case 0x4C: return z80BitNPtrIxIy();     // 01001100: BIT 1, (In + d)
        case 0x4D: return z80BitNPtrIxIy();     // 01001101: BIT 1, (In + d)
        case 0x4E: return z80BitNPtrIxIy();     // 01001110: BIT 1, (In + d)
        case 0x4F: return z80BitNPtrIxIy();     // 01001111: BIT 1, (In + d)
        case 0x50: return z80BitNPtrIxIy();     // 01010000: BIT 2, (In + d)
        case 0x51: return z80BitNPtrIxIy();     // 01010001: BIT 2, (In + d)
        case 0x52: return z80BitNPtrIxIy();     // 01010010: BIT 2, (In + d)
        case 0x53: return z80BitNPtrIxIy();     // 01010011: BIT 2, (In + d)
        case 0x54: return z80BitNPtrIxIy();     // 01010100: BIT 2, (In + d)
        case 0x55: return z80BitNPtrIxIy();     // 01010101: BIT 2, (In + d)
        case 0x56: return z80BitNPtrIxIy();     // 01010110: BIT 2, (In + d)
        case 0x57: return z80BitNPtrIxIy();     // 01010111: BIT 2, (In + d)
        case 0x58: return z80BitNPtrIxIy();     // 01011000: BIT 3, (In + d)
        case 0x59: return z80BitNPtrIxIy();     // 01011001: BIT 3, (In + d)
        case 0x5A: return z80BitNPtrIxIy();     // 01011010: BIT 3, (In + d)
        case 0x5B: return z80BitNPtrIxIy();     // 01011011: BIT 3, (In + d)
        case 0x5C: return z80BitNPtrIxIy();     // 01011100: BIT 3, (In + d)
        case 0x5D: return z80BitNPtrIxIy();     // 01011101: BIT 3, (In + d)
        case 0x5E: return z80BitNPtrIxIy();     // 01011110: BIT 3, (In + d)
        case 0x5F: return z80BitNPtrIxIy();     // 01011111: BIT 3, (In + d)
        case 0x60: return z80BitNPtrIxIy();     // 01100000: BIT 4, (In + d)
        case 0x61: return z80BitNPtrIxIy();     // 01100001: BIT 4, (In + d)
        case 0x62: return z80BitNPtrIxIy();     // 01100010: BIT 4, (In + d)
        case 0x63: return z80BitNPtrIxIy();     // 01100011: BIT 4, (In + d)
        case 0x64: return z80BitNPtrIxIy();     // 01100100: BIT 4, (In + d)
        case 0x65: return z80BitNPtrIxIy();     // 01100101: BIT 4, (In + d)
        case 0x66: return z80BitNPtrIxIy();     // 01100110: BIT 4, (In + d)
        case 0x67: return z80BitNPtrIxIy();     // 01100111: BIT 4, (In + d)
        case 0x68: return z80BitNPtrIxIy();     // 01101000: BIT 5, (In + d)
        case 0x69: return z80BitNPtrIxIy();     // 01101001: BIT 5, (In + d)
        case 0x6A: return z80BitNPtrIxIy();     // 01101010: BIT 5, (In + d)
        case 0x6B: return z80BitNPtrIxIy();     // 01101011: BIT 5, (In + d)
        case 0x6C: return z80BitNPtrIxIy();     // 01101100: BIT 5, (In + d)
        case 0x6D: return z80BitNPtrIxIy();     // 01101101: BIT 5, (In + d)
        case 0x6E: return z80BitNPtrIxIy();     // 01101110: BIT 5, (In + d)
        case 0x6F: return z80BitNPtrIxIy();     // 01101111: BIT 5, (In + d)
        case 0x70: return z80BitNPtrIxIy();     // 01110000: BIT 6, (In + d)
        case 0x71: return z80BitNPtrIxIy();     // 01110001: BIT 6, (In + d)
        case 0x72: return z80BitNPtrIxIy();     // 01110010: BIT 6, (In + d)
        case 0x73: return z80BitNPtrIxIy();     // 01110011: BIT 6, (In + d)
        case 0x74: return z80BitNPtrIxIy();     // 01110100: BIT 6, (In + d)
        case 0x75: return z80BitNPtrIxIy();     // 01110101: BIT 6, (In + d)
        case 0x76: return z80BitNPtrIxIy();     // 01110110: BIT 6, (In + d)
        case 0x77: return z80BitNPtrIxIy();     // 01110111: BIT 6, (In + d)
        case 0x78: return z80BitNPtrIxIy();     // 01111000: BIT 7, (In + d)
        case 0x79: return z80BitNPtrIxIy();     // 01111001: BIT 7, (In + d)
        case 0x7A: return z80BitNPtrIxIy();     // 01111010: BIT 7, (In + d)
        case 0x7B: return z80BitNPtrIxIy();     // 01111011: BIT 7, (In + d)
        case 0x7C: return z80BitNPtrIxIy();     // 01111100: BIT 7, (In + d)
        case 0x7D: return z80BitNPtrIxIy();     // 01111101: BIT 7, (In + d)
        case 0x7E: return z80BitNPtrIxIy();     // 01111110: BIT 7, (In + d)
        case 0x7F: return z80BitNPtrIxIy();     // 01111111: BIT 7, (In + d)
        case 0x80: return z80ResNPtrIxIy();     // 10000000: RES 0, (In + d), B
        case 0x81: return z80ResNPtrIxIy();     // 10000001: RES 0, (In + d), C
        case 0x82: return z80ResNPtrIxIy();     // 10000010: RES 0, (In + d), D
        case 0x83: return z80ResNPtrIxIy();     // 10000011: RES 0, (In + d), E
        case 0x84: return z80ResNPtrIxIy();     // 10000100: RES 0, (In + d), H
        case 0x85: return z80ResNPtrIxIy();     // 10000101: RES 0, (In + d), L
        case 0x86: return z80ResNPtrIxIy();     // 10000110: RES 0, (In + d)
        case 0x87: return z80ResNPtrIxIy();     // 10000111: RES 0, (In + d), A
        case 0x88: return z80ResNPtrIxIy();     // 10001000: RES 1, (In + d), B
        case 0x89: return z80ResNPtrIxIy();     // 10001001: RES 1, (In + d), C
        case 0x8A: return z80ResNPtrIxIy();     // 10001010: RES 1, (In + d), D
        case 0x8B: return z80ResNPtrIxIy();     // 10001011: RES 1, (In + d), E
        case 0x8C: return z80ResNPtrIxIy();     // 10001100: RES 1, (In + d), H
        case 0x8D: return z80ResNPtrIxIy();     // 10001101: RES 1, (In + d), L
        case 0x8E: return z80ResNPtrIxIy();     // 10001110: RES 1, (In + d)
        case 0x8F: return z80ResNPtrIxIy();     // 10001111: RES 1, (In + d), A
        case 0x90: return z80ResNPtrIxIy();     // 10010000: RES 2, (In + d), B
        case 0x91: return z80ResNPtrIxIy();     // 10010001: RES 2, (In + d), C
        case 0x92: return z80ResNPtrIxIy();     // 10010010: RES 2, (In + d), D
        case 0x93: return z80ResNPtrIxIy();     // 10010011: RES 2, (In + d), E
        case 0x94: return z80ResNPtrIxIy();     // 10010100: RES 2, (In + d), H
        case 0x95: return z80ResNPtrIxIy();     // 10010101: RES 2, (In + d), L
        case 0x96: return z80ResNPtrIxIy();     // 10010110: RES 2, (In + d)
        case 0x97: return z80ResNPtrIxIy();     // 10010111: RES 2, (In + d), A
        case 0x98: return z80ResNPtrIxIy();     // 10011000: RES 3, (In + d), B
        case 0x99: return z80ResNPtrIxIy();     // 10011001: RES 3, (In + d), C
        case 0x9A: return z80ResNPtrIxIy();     // 10011010: RES 3, (In + d), D
        case 0x9B: return z80ResNPtrIxIy();     // 10011011: RES 3, (In + d), E
        case 0x9C: return z80ResNPtrIxIy();     // 10011100: RES 3, (In + d), H
        case 0x9D: return z80ResNPtrIxIy();     // 10011101: RES 3, (In + d), L
        case 0x9E: return z80ResNPtrIxIy();     // 10011110: RES 3, (In + d)
        case 0x9F: return z80ResNPtrIxIy();     // 10011111: RES 3, (In + d), A
        case 0xA0: return z80ResNPtrIxIy();     // 10100000: RES 4, (In + d), B
        case 0xA1: return z80ResNPtrIxIy();     // 10100001: RES 4, (In + d), C
        case 0xA2: return z80ResNPtrIxIy();     // 10100010: RES 4, (In + d), D
        case 0xA3: return z80ResNPtrIxIy();     // 10100011: RES 4, (In + d), E
        case 0xA4: return z80ResNPtrIxIy();     // 10100100: RES 4, (In + d), H
        case 0xA5: return z80ResNPtrIxIy();     // 10100101: RES 4, (In + d), L
        case 0xA6: return z80ResNPtrIxIy();     // 10100110: RES 4, (In + d)
        case 0xA7: return z80ResNPtrIxIy();     // 10100111: RES 4, (In + d), A
        case 0xA8: return z80ResNPtrIxIy();     // 10101000: RES 5, (In + d), B
        case 0xA9: return z80ResNPtrIxIy();     // 10101001: RES 5, (In + d), C
        case 0xAA: return z80ResNPtrIxIy();     // 10101010: RES 5, (In + d), D
        case 0xAB: return z80ResNPtrIxIy();     // 10101011: RES 5, (In + d), E
        case 0xAC: return z80ResNPtrIxIy();     // 10101100: RES 5, (In + d), H
        case 0xAD: return z80ResNPtrIxIy();     // 10101101: RES 5, (In + d), L
        case 0xAE: return z80ResNPtrIxIy();     // 10101110: RES 5, (In + d)
        case 0xAF: return z80ResNPtrIxIy();     // 10101111: RES 5, (In + d), A
        case 0xB0: return z80ResNPtrIxIy();     // 10110000: RES 6, (In + d), B
        case 0xB1: return z80ResNPtrIxIy();     // 10110001: RES 6, (In + d), C
        case 0xB2: return z80ResNPtrIxIy();     // 10110010: RES 6, (In + d), D
        case 0xB3: return z80ResNPtrIxIy();     // 10110011: RES 6, (In + d), E
        case 0xB4: return z80ResNPtrIxIy();     // 10110100: RES 6, (In + d), H
        case 0xB5: return z80ResNPtrIxIy();     // 10110101: RES 6, (In + d), L
        case 0xB6: return z80ResNPtrIxIy();     // 10110110: RES 6, (In + d)
        case 0xB7: return z80ResNPtrIxIy();     // 10110111: RES 6, (In + d), A
        case 0xB8: return z80ResNPtrIxIy();     // 10111000: RES 7, (In + d), B
        case 0xB9: return z80ResNPtrIxIy();     // 10111001: RES 7, (In + d), C
        case 0xBA: return z80ResNPtrIxIy();     // 10111010: RES 7, (In + d), D
        case 0xBB: return z80ResNPtrIxIy();     // 10111011: RES 7, (In + d), E
        case 0xBC: return z80ResNPtrIxIy();     // 10111100: RES 7, (In + d), H
        case 0xBD: return z80ResNPtrIxIy();     // 10111101: RES 7, (In + d), L
        case 0xBE: return z80ResNPtrIxIy();     // 10111110: RES 7, (In + d)
        case 0xBF: return z80ResNPtrIxIy();     // 10111111: RES 7, (In + d), A
        case 0xC0: return z80SetNPtrIxIy();     // 11000000: SET 0, (In + d), B
        case 0xC1: return z80SetNPtrIxIy();     // 11000001: SET 0, (In + d), C
        case 0xC2: return z80SetNPtrIxIy();     // 11000010: SET 0, (In + d), D
        case 0xC3: return z80SetNPtrIxIy();     // 11000011: SET 0, (In + d), E
        case 0xC4: return z80SetNPtrIxIy();     // 11000100: SET 0, (In + d), H
        case 0xC5: return z80SetNPtrIxIy();     // 11000101: SET 0, (In + d), L
        case 0xC6: return z80SetNPtrIxIy();     // 11000110: SET 0, (In + d)
        case 0xC7: return z80SetNPtrIxIy();     // 11000111: SET 0, (In + d), A
        case 0xC8: return z80SetNPtrIxIy();     // 11001000: SET 1, (In + d), B
        case 0xC9: return z80SetNPtrIxIy();     // 11001001: SET 1, (In + d), C
        case 0xCA: return z80SetNPtrIxIy();     // 11001010: SET 1, (In + d), D
        case 0xCB: return z80SetNPtrIxIy();     // 11001011: SET 1, (In + d), E
        case 0xCC: return z80SetNPtrIxIy();     // 11001100: SET 1, (In + d), H
        case 0xCD: return z80SetNPtrIxIy();     // 11001101: SET 1, (In + d), L
        case 0xCE: return z80SetNPtrIxIy();     // 11001110: SET 1, (In + d)
        case 0xCF: return z80SetNPtrIxIy();     // 11001111: SET 1, (In + d), A
        case 0xD0: return z80SetNPtrIxIy();     // 11010000: SET 2, (In + d), B
        case 0xD1: return z80SetNPtrIxIy();     // 11010001: SET 2, (In + d), C
        case 0xD2: return z80SetNPtrIxIy();     // 11010010: SET 2, (In + d), D
        case 0xD3: return z80SetNPtrIxIy();     // 11010011: SET 2, (In + d), E
        case 0xD4: return z80SetNPtrIxIy();     // 11010100: SET 2, (In + d), H
        case 0xD5: return z80SetNPtrIxIy();     // 11010101: SET 2, (In + d), L
        case 0xD6: return z80SetNPtrIxIy();     // 11010110: SET 2, (In + d)
        case 0xD7: return z80SetNPtrIxIy();     // 11010111: SET 2, (In + d), A
        case 0xD8: return z80SetNPtrIxIy();     // 11011000: SET 3, (In + d), B
        case 0xD9: return z80SetNPtrIxIy();     // 11011001: SET 3, (In + d), C
        case 0xDA: return z80SetNPtrIxIy();     // 11011010: SET 3, (In + d), D
        case 0xDB: return z80SetNPtrIxIy();     // 11011011: SET 3, (In + d), E
        case 0xDC: return z80SetNPtrIxIy();     // 11011100: SET 3, (In + d), H
        case 0xDD: return z80SetNPtrIxIy();     // 11011101: SET 3, (In + d), L
        case 0xDE: return z80SetNPtrIxIy();     // 11011110: SET 3, (In + d)
        case 0xDF: return z80SetNPtrIxIy();     // 11011111: SET 3, (In + d), A
        case 0xE0: return z80SetNPtrIxIy();     // 11100000: SET 4, (In + d), B
        case 0xE1: return z80SetNPtrIxIy();     // 11100001: SET 4, (In + d), C
        case 0xE2: return z80SetNPtrIxIy();     // 11100010: SET 4, (In + d), D
        case 0xE3: return z80SetNPtrIxIy();     // 11100011: SET 4, (In + d), E
        case 0xE4: return z80SetNPtrIxIy();     // 11100100: SET 4, (In + d), H
        case 0xE5: return z80SetNPtrIxIy();     // 11100101: SET 4, (In + d), L
        case 0xE6: return z80SetNPtrIxIy();     // 11100110: SET 4, (In + d)
        case 0xE7: return z80SetNPtrIxIy();     // 11100111: SET 4, (In + d), A
        case 0xE8: return z80SetNPtrIxIy();     // 11101000: SET 5, (In + d), B
        case 0xE9: return z80SetNPtrIxIy();     // 11101001: SET 5, (In + d), C
        case 0xEA: return z80SetNPtrIxIy();     // 11101010: SET 5, (In + d), D
        case 0xEB: return z80SetNPtrIxIy();     // 11101011: SET 5, (In + d), E
        case 0xEC: return z80SetNPtrIxIy();     // 11101100: SET 5, (In + d), H
        case 0xED: return z80SetNPtrIxIy();     // 11101101: SET 5, (In + d), L
        case 0xEE: return z80SetNPtrIxIy();     // 11101110: SET 5, (In + d)
        case 0xEF: return z80SetNPtrIxIy();     // 11101111: SET 5, (In + d), A
        case 0xF0: return z80SetNPtrIxIy();     // 11110000: SET 6, (In + d), B
        case 0xF1: return z80SetNPtrIxIy();     // 11110001: SET 6, (In + d), C
        case 0xF2: return z80SetNPtrIxIy();     // 11110010: SET 6, (In + d), D
        case 0xF3: return z80SetNPtrIxIy();     // 11110011: SET 6, (In + d), E
        case 0xF4: return z80SetNPtrIxIy();     // 11110100: SET 6, (In + d), H
        case 0xF5: return z80SetNPtrIxIy();     // 11110101: SET 6, (In + d), L
        case 0xF6: return z80SetNPtrIxIy();     // 11110110: SET 6, (In + d)
        case 0xF7: return z80SetNPtrIxIy();     // 11110111: SET 6, (In + d), A
        case 0xF8: return z80SetNPtrIxIy();     // 11111000: SET 7, (In + d), B
        case 0xF9: return z80SetNPtrIxIy();     // 11111001: SET 7, (In + d), C
        case 0xFA: return z80SetNPtrIxIy();     // 11111010: SET 7, (In + d), D
        case 0xFB: return z80SetNPtrIxIy();     // 11111011: SET 7, (In + d), E
        case 0xFC: return z80SetNPtrIxIy();     // 11111100: SET 7, (In + d), H
        case 0xFD: return z80SetNPtrIxIy();     // 11111101: SET 7, (In + d), L
        case 0xFE: return z80SetNPtrIxIy();     // 11111110: SET 7, (In + d)
        case 0xFF: return z80SetNPtrIxIy();     // 11111111: SET 7, (In + d), A
        default: assert(false); return true;   // Should not happen
    }
}
*/

// vim: et:sw=4:ts=4
