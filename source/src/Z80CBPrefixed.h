#pragma once

/** Z80CBPrefixed.h
 *
 * Z80 instruction table for CB-prefixed opcodes.
 *
 */

#include "Z80Nop.h"

#include "Z80RlcReg.h"
#include "Z80RrcReg.h"
#include "Z80RlReg.h"
#include "Z80RrReg.h"
#include "Z80SlaReg.h"
#include "Z80SraReg.h"
#include "Z80SllReg.h"
#include "Z80SrlReg.h"

#include "Z80RlcPtrHl.h"
#include "Z80RrcPtrHl.h"
#include "Z80RlPtrHl.h"
#include "Z80RrPtrHl.h"
#include "Z80SlaPtrHl.h"
#include "Z80SraPtrHl.h"
#include "Z80SllPtrHl.h"
#include "Z80SrlPtrHl.h"

#include "Z80BitNReg.h"
#include "Z80BitNPtrHl.h"
#include "Z80ResNReg.h"
#include "Z80ResNPtrHl.h"
#include "Z80SetNReg.h"
#include "Z80SetNPtrHl.h"

bool z80CBPrefixed(uint_fast8_t opcode)
{
    switch (opcode)
    {
        case 0x00: return z80RlcReg();          // 00000000: RLC B
        case 0x01: return z80RlcReg();          // 00000001: RLC C
        case 0x02: return z80RlcReg();          // 00000010: RLC D
        case 0x03: return z80RlcReg();          // 00000011: RLC E
        case 0x04: return z80RlcReg();          // 00000100: RLC H
        case 0x05: return z80RlcReg();          // 00000101: RLC L
        case 0x06: return z80RlcPtrHl();        // 00000110: RLC (HL)
        case 0x07: return z80RlcReg();          // 00000111: RLC A
        case 0x08: return z80RrcReg();          // 00001000: RRC B
        case 0x09: return z80RrcReg();          // 00001001: RRC C
        case 0x0A: return z80RrcReg();          // 00001010: RRC D
        case 0x0B: return z80RrcReg();          // 00001011: RRC E
        case 0x0C: return z80RrcReg();          // 00001100: RRC H
        case 0x0D: return z80RrcReg();          // 00001101: RRC L
        case 0x0E: return z80RrcPtrHl();        // 00001110: RRC (HL)
        case 0x0F: return z80RrcReg();          // 00001111: RRC A
        case 0x10: return z80RlReg();           // 00010000: RL B
        case 0x11: return z80RlReg();           // 00010001: RL C
        case 0x12: return z80RlReg();           // 00010010: RL D
        case 0x13: return z80RlReg();           // 00010011: RL E
        case 0x14: return z80RlReg();           // 00010100: RL H
        case 0x15: return z80RlReg();           // 00010101: RL L
        case 0x16: return z80RlPtrHl();         // 00010110: RL (HL)
        case 0x17: return z80RlReg();           // 00010111: RL A
        case 0x18: return z80RrReg();           // 00011000: RR B
        case 0x19: return z80RrReg();           // 00011001: RR C
        case 0x1A: return z80RrReg();           // 00011010: RR D
        case 0x1B: return z80RrReg();           // 00011011: RR E
        case 0x1C: return z80RrReg();           // 00011100: RR H
        case 0x1D: return z80RrReg();           // 00011101: RR L
        case 0x1E: return z80RrPtrHl();         // 00011110: RR (HL)
        case 0x1F: return z80RrReg();           // 00011111: RR A
        case 0x20: return z80SlaReg();          // 00100000: SLA B
        case 0x21: return z80SlaReg();          // 00100001: SLA C
        case 0x22: return z80SlaReg();          // 00100010: SLA D
        case 0x23: return z80SlaReg();          // 00100011: SLA E
        case 0x24: return z80SlaReg();          // 00100100: SLA H
        case 0x25: return z80SlaReg();          // 00100101: SLA L
        case 0x26: return z80SlaPtrHl();        // 00100110: SLA (HL)
        case 0x27: return z80SlaReg();          // 00100111: SLA A
        case 0x28: return z80SraReg();          // 00101000: SRA B
        case 0x29: return z80SraReg();          // 00101001: SRA C
        case 0x2A: return z80SraReg();          // 00101010: SRA D
        case 0x2B: return z80SraReg();          // 00101011: SRA E
        case 0x2C: return z80SraReg();          // 00101100: SRA H
        case 0x2D: return z80SraReg();          // 00101101: SRA L
        case 0x2E: return z80SraPtrHl();        // 00101110: SRA (HL)
        case 0x2F: return z80SraReg();          // 00101111: SRA A
        case 0x30: return z80SllReg();          // 00110000: SLL B
        case 0x31: return z80SllReg();          // 00110001: SLL C
        case 0x32: return z80SllReg();          // 00110010: SLL D
        case 0x33: return z80SllReg();          // 00110011: SLL E
        case 0x34: return z80SllReg();          // 00110100: SLL H
        case 0x35: return z80SllReg();          // 00100101: SLL L
        case 0x36: return z80SllPtrHl();        // 00100110: SLL (HL)
        case 0x37: return z80SllReg();          // 00100111: SLL A
        case 0x38: return z80SrlReg();          // 00111000: SRL B
        case 0x39: return z80SrlReg();          // 00111001: SRL C
        case 0x3A: return z80SrlReg();          // 00111010: SRL D
        case 0x3B: return z80SrlReg();          // 00111011: SRL E
        case 0x3C: return z80SrlReg();          // 00111100: SRL H
        case 0x3D: return z80SrlReg();          // 00111101: SRL L
        case 0x3E: return z80SrlPtrHl();        // 00111110: SRL (HL)
        case 0x3F: return z80SrlReg();          // 00111111: SRL A
        case 0x40: return z80BitNReg();         // 01000000: BIT 0, B
        case 0x41: return z80BitNReg();         // 01000001: BIT 0, C
        case 0x42: return z80BitNReg();         // 01000010: BIT 0, D
        case 0x43: return z80BitNReg();         // 01000011: BIT 0, E
        case 0x44: return z80BitNReg();         // 01000100: BIT 0, H
        case 0x45: return z80BitNReg();         // 01000101: BIT 0, L
        case 0x46: return z80BitNPtrHl();       // 01000110: BIT 0, (HL)
        case 0x47: return z80BitNReg();         // 01000111: BIT 0, A
        case 0x48: return z80BitNReg();         // 01001000: BIT 1, B
        case 0x49: return z80BitNReg();         // 01001001: BIT 1, C
        case 0x4A: return z80BitNReg();         // 01001010: BIT 1, D
        case 0x4B: return z80BitNReg();         // 01001011: BIT 1, E
        case 0x4C: return z80BitNReg();         // 01001100: BIT 1, H
        case 0x4D: return z80BitNReg();         // 01001101: BIT 1, L
        case 0x4E: return z80BitNPtrHl();       // 01001110: BIT 1, (HL)
        case 0x4F: return z80BitNReg();         // 01001111: BIT 1, A
        case 0x50: return z80BitNReg();         // 01010000: BIT 2, B
        case 0x51: return z80BitNReg();         // 01010001: BIT 2, C
        case 0x52: return z80BitNReg();         // 01010010: BIT 2, D
        case 0x53: return z80BitNReg();         // 01010011: BIT 2, E
        case 0x54: return z80BitNReg();         // 01010100: BIT 2, H
        case 0x55: return z80BitNReg();         // 01010101: BIT 2, L
        case 0x56: return z80BitNPtrHl();       // 01010110: BIT 2, (HL)
        case 0x57: return z80BitNReg();         // 01010111: BIT 2, A
        case 0x58: return z80BitNReg();         // 01011000: BIT 3, B
        case 0x59: return z80BitNReg();         // 01011001: BIT 3, C
        case 0x5A: return z80BitNReg();         // 01011010: BIT 3, D
        case 0x5B: return z80BitNReg();         // 01011011: BIT 3, E
        case 0x5C: return z80BitNReg();         // 01011100: BIT 3, H
        case 0x5D: return z80BitNReg();         // 01011101: BIT 3, L
        case 0x5E: return z80BitNPtrHl();       // 01011110: BIT 3, (HL)
        case 0x5F: return z80BitNReg();         // 01011111: BIT 3, A
        case 0x60: return z80BitNReg();         // 01100000: BIT 4, B
        case 0x61: return z80BitNReg();         // 01100001: BIT 4, C
        case 0x62: return z80BitNReg();         // 01100010: BIT 4, D
        case 0x63: return z80BitNReg();         // 01100011: BIT 4, E
        case 0x64: return z80BitNReg();         // 01100100: BIT 4, H
        case 0x65: return z80BitNReg();         // 01100101: BIT 4, L
        case 0x66: return z80BitNPtrHl();       // 01100110: BIT 4, (HL)
        case 0x67: return z80BitNReg();         // 01100111: BIT 4, A
        case 0x68: return z80BitNReg();         // 01101000: BIT 5, B
        case 0x69: return z80BitNReg();         // 01101001: BIT 5, C
        case 0x6A: return z80BitNReg();         // 01101010: BIT 5, D
        case 0x6B: return z80BitNReg();         // 01101011: BIT 5, E
        case 0x6C: return z80BitNReg();         // 01101100: BIT 5, H
        case 0x6D: return z80BitNReg();         // 01101101: BIT 5, L
        case 0x6E: return z80BitNPtrHl();       // 01101110: BIT 5, (HL)
        case 0x6F: return z80BitNReg();         // 01101111: BIT 5, A
        case 0x70: return z80BitNReg();         // 01110000: BIT 6, B
        case 0x71: return z80BitNReg();         // 01110001: BIT 6, C
        case 0x72: return z80BitNReg();         // 01110010: BIT 6, D
        case 0x73: return z80BitNReg();         // 01110011: BIT 6, E
        case 0x74: return z80BitNReg();         // 01110100: BIT 6, H
        case 0x75: return z80BitNReg();         // 01110101: BIT 6, L
        case 0x76: return z80BitNPtrHl();       // 01110110: BIT 6, (HL)
        case 0x77: return z80BitNReg();         // 01110111: BIT 6, A
        case 0x78: return z80BitNReg();         // 01111000: BIT 7, B
        case 0x79: return z80BitNReg();         // 01111001: BIT 7, C
        case 0x7A: return z80BitNReg();         // 01111010: BIT 7, D
        case 0x7B: return z80BitNReg();         // 01111011: BIT 7, E
        case 0x7C: return z80BitNReg();         // 01111100: BIT 7, H
        case 0x7D: return z80BitNReg();         // 01111101: BIT 7, L
        case 0x7E: return z80BitNPtrHl();       // 01111110: BIT 7, (HL)
        case 0x7F: return z80BitNReg();         // 01111111: BIT 7, A
        case 0x80: return z80ResNReg();         // 10000000: RES 0, B
        case 0x81: return z80ResNReg();         // 10000001: RES 0, C
        case 0x82: return z80ResNReg();         // 10000010: RES 0, D
        case 0x83: return z80ResNReg();         // 10000011: RES 0, E
        case 0x84: return z80ResNReg();         // 10000100: RES 0, H
        case 0x85: return z80ResNReg();         // 10000101: RES 0, L
        case 0x86: return z80ResNPtrHl();       // 10000110: RES 0, (HL)
        case 0x87: return z80ResNReg();         // 10000111: RES 0, A
        case 0x88: return z80ResNReg();         // 10001000: RES 1, B
        case 0x89: return z80ResNReg();         // 10001001: RES 1, C
        case 0x8A: return z80ResNReg();         // 10001010: RES 1, D
        case 0x8B: return z80ResNReg();         // 10001011: RES 1, E
        case 0x8C: return z80ResNReg();         // 10001100: RES 1, H
        case 0x8D: return z80ResNReg();         // 10001101: RES 1, L
        case 0x8E: return z80ResNPtrHl();       // 10001110: RES 1, (HL)
        case 0x8F: return z80ResNReg();         // 10001111: RES 1, A
        case 0x90: return z80ResNReg();         // 10010000: RES 2, B
        case 0x91: return z80ResNReg();         // 10010001: RES 2, C
        case 0x92: return z80ResNReg();         // 10010010: RES 2, D
        case 0x93: return z80ResNReg();         // 10010011: RES 2, E
        case 0x94: return z80ResNReg();         // 10010100: RES 2, H
        case 0x95: return z80ResNReg();         // 10010101: RES 2, L
        case 0x96: return z80ResNPtrHl();       // 10010110: RES 2, (HL)
        case 0x97: return z80ResNReg();         // 10010111: RES 2, A
        case 0x98: return z80ResNReg();         // 10011000: RES 3, B
        case 0x99: return z80ResNReg();         // 10011001: RES 3, C
        case 0x9A: return z80ResNReg();         // 10011010: RES 3, D
        case 0x9B: return z80ResNReg();         // 10011011: RES 3, E
        case 0x9C: return z80ResNReg();         // 10011100: RES 3, H
        case 0x9D: return z80ResNReg();         // 10011101: RES 3, L
        case 0x9E: return z80ResNPtrHl();       // 10011110: RES 3, (HL)
        case 0x9F: return z80ResNReg();         // 10011111: RES 3, A
        case 0xA0: return z80ResNReg();         // 10100000: RES 4, B
        case 0xA1: return z80ResNReg();         // 10100001: RES 4, C
        case 0xA2: return z80ResNReg();         // 10100010: RES 4, D
        case 0xA3: return z80ResNReg();         // 10100011: RES 4, E
        case 0xA4: return z80ResNReg();         // 10100100: RES 4, H
        case 0xA5: return z80ResNReg();         // 10100101: RES 4, L
        case 0xA6: return z80ResNPtrHl();       // 10100110: RES 4, (HL)
        case 0xA7: return z80ResNReg();         // 10100111: RES 4, A
        case 0xA8: return z80ResNReg();         // 10101000: RES 5, B
        case 0xA9: return z80ResNReg();         // 10101001: RES 5, C
        case 0xAA: return z80ResNReg();         // 10101010: RES 5, D
        case 0xAB: return z80ResNReg();         // 10101011: RES 5, E
        case 0xAC: return z80ResNReg();         // 10101100: RES 5, H
        case 0xAD: return z80ResNReg();         // 10101101: RES 5, L
        case 0xAE: return z80ResNPtrHl();       // 10101110: RES 5, (HL)
        case 0xAF: return z80ResNReg();         // 10101111: RES 5, A
        case 0xB0: return z80ResNReg();         // 10110000: RES 6, B
        case 0xB1: return z80ResNReg();         // 10110001: RES 6, C
        case 0xB2: return z80ResNReg();         // 10110010: RES 6, D
        case 0xB3: return z80ResNReg();         // 10110011: RES 6, E
        case 0xB4: return z80ResNReg();         // 10110100: RES 6, H
        case 0xB5: return z80ResNReg();         // 10110101: RES 6, L
        case 0xB6: return z80ResNPtrHl();       // 10110110: RES 6, (HL)
        case 0xB7: return z80ResNReg();         // 10110111: RES 6, A
        case 0xB8: return z80ResNReg();         // 10111000: RES 7, B
        case 0xB9: return z80ResNReg();         // 10111001: RES 7, C
        case 0xBA: return z80ResNReg();         // 10111010: RES 7, D
        case 0xBB: return z80ResNReg();         // 10111011: RES 7, E
        case 0xBC: return z80ResNReg();         // 10111100: RES 7, H
        case 0xBD: return z80ResNReg();         // 10111101: RES 7, L
        case 0xBE: return z80ResNPtrHl();       // 10111110: RES 7, (HL)
        case 0xBF: return z80ResNReg();         // 10111111: RES 7, A
        case 0xC0: return z80SetNReg();         // 11000000: SET 0, B
        case 0xC1: return z80SetNReg();         // 11000001: SET 0, C
        case 0xC2: return z80SetNReg();         // 11000010: SET 0, D
        case 0xC3: return z80SetNReg();         // 11000011: SET 0, E
        case 0xC4: return z80SetNReg();         // 11000100: SET 0, H
        case 0xC5: return z80SetNReg();         // 11000101: SET 0, L
        case 0xC6: return z80SetNPtrHl();       // 11000110: SET 0, (HL)
        case 0xC7: return z80SetNReg();         // 11000111: SET 0, A
        case 0xC8: return z80SetNReg();         // 11001000: SET 1, B
        case 0xC9: return z80SetNReg();         // 11001001: SET 1, C
        case 0xCA: return z80SetNReg();         // 11001010: SET 1, D
        case 0xCB: return z80SetNReg();         // 11001011: SET 1, E
        case 0xCC: return z80SetNReg();         // 11001100: SET 1, H
        case 0xCD: return z80SetNReg();         // 11001101: SET 1, L
        case 0xCE: return z80SetNPtrHl();       // 11001110: SET 1, (HL)
        case 0xCF: return z80SetNReg();         // 11001111: SET 1, A
        case 0xD0: return z80SetNReg();         // 11010000: SET 2, B
        case 0xD1: return z80SetNReg();         // 11010001: SET 2, C
        case 0xD2: return z80SetNReg();         // 11010010: SET 2, D
        case 0xD3: return z80SetNReg();         // 11010011: SET 2, E
        case 0xD4: return z80SetNReg();         // 11010100: SET 2, H
        case 0xD5: return z80SetNReg();         // 11010101: SET 2, L
        case 0xD6: return z80SetNPtrHl();       // 11010110: SET 2, (HL)
        case 0xD7: return z80SetNReg();         // 11010111: SET 2, A
        case 0xD8: return z80SetNReg();         // 11011000: SET 3, B
        case 0xD9: return z80SetNReg();         // 11011001: SET 3, C
        case 0xDA: return z80SetNReg();         // 11011010: SET 3, D
        case 0xDB: return z80SetNReg();         // 11011011: SET 3, E
        case 0xDC: return z80SetNReg();         // 11011100: SET 3, H
        case 0xDD: return z80SetNReg();         // 11011101: SET 3, L
        case 0xDE: return z80SetNPtrHl();       // 11011110: SET 3, (HL)
        case 0xDF: return z80SetNReg();         // 11011111: SET 3, A
        case 0xE0: return z80SetNReg();         // 11100000: SET 4, B
        case 0xE1: return z80SetNReg();         // 11100001: SET 4, C
        case 0xE2: return z80SetNReg();         // 11100010: SET 4, D
        case 0xE3: return z80SetNReg();         // 11100011: SET 4, E
        case 0xE4: return z80SetNReg();         // 11100100: SET 4, H
        case 0xE5: return z80SetNReg();         // 11100101: SET 4, L
        case 0xE6: return z80SetNPtrHl();       // 11100110: SET 4, (HL)
        case 0xE7: return z80SetNReg();         // 11100111: SET 4, A
        case 0xE8: return z80SetNReg();         // 11101000: SET 5, B
        case 0xE9: return z80SetNReg();         // 11101001: SET 5, C
        case 0xEA: return z80SetNReg();         // 11101010: SET 5, D
        case 0xEB: return z80SetNReg();         // 11101011: SET 5, E
        case 0xEC: return z80SetNReg();         // 11101100: SET 5, H
        case 0xED: return z80SetNReg();         // 11101101: SET 5, L
        case 0xEE: return z80SetNPtrHl();       // 11101110: SET 5, (HL)
        case 0xEF: return z80SetNReg();         // 11101111: SET 5, A
        case 0xF0: return z80SetNReg();         // 11110000: SET 6, B
        case 0xF1: return z80SetNReg();         // 11110001: SET 6, C
        case 0xF2: return z80SetNReg();         // 11110010: SET 6, D
        case 0xF3: return z80SetNReg();         // 11110011: SET 6, E
        case 0xF4: return z80SetNReg();         // 11110100: SET 6, H
        case 0xF5: return z80SetNReg();         // 11110101: SET 6, L
        case 0xF6: return z80SetNPtrHl();       // 11110110: SET 6, (HL)
        case 0xF7: return z80SetNReg();         // 11110111: SET 6, A
        case 0xF8: return z80SetNReg();         // 11111000: SET 7, B
        case 0xF9: return z80SetNReg();         // 11111001: SET 7, C
        case 0xFA: return z80SetNReg();         // 11111010: SET 7, D
        case 0xFB: return z80SetNReg();         // 11111011: SET 7, E
        case 0xFC: return z80SetNReg();         // 11111100: SET 7, H
        case 0xFD: return z80SetNReg();         // 11111101: SET 7, L
        case 0xFE: return z80SetNPtrHl();       // 11111110: SET 7, (HL)
        case 0xFF: return z80SetNReg();         // 11111111: SET 7, A
        default: assert(false); return true;   // Should not happen
    }
}

// vim: et:sw=4:ts=4
