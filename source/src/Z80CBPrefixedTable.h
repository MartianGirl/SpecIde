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

cbprefixed
{
    &Z80::z80RlcReg,                  // 00000000: RLC B
    &Z80::z80RlcReg,                  // 00000001: RLC C
    &Z80::z80RlcReg,                  // 00000010: RLC D
    &Z80::z80RlcReg,                  // 00000011: RLC E
    &Z80::z80RlcReg,                  // 00000100: RLC H
    &Z80::z80RlcReg,                  // 00000101: RLC L
    &Z80::z80RlcPtrHl,                // 00000110: RLC (HL)
    &Z80::z80RlcReg,                  // 00000111: RLC A
    &Z80::z80RrcReg,                  // 00001000: RRC B
    &Z80::z80RrcReg,                  // 00001001: RRC C
    &Z80::z80RrcReg,                  // 00001010: RRC D
    &Z80::z80RrcReg,                  // 00001011: RRC E
    &Z80::z80RrcReg,                  // 00001100: RRC H
    &Z80::z80RrcReg,                  // 00001101: RRC L
    &Z80::z80RrcPtrHl,                // 00001110: RRC (HL)
    &Z80::z80RrcReg,                  // 00001111: RRC A
    &Z80::z80RlReg,                   // 00010000: RL B
    &Z80::z80RlReg,                   // 00010001: RL C
    &Z80::z80RlReg,                   // 00010010: RL D
    &Z80::z80RlReg,                   // 00010011: RL E
    &Z80::z80RlReg,                   // 00010100: RL H
    &Z80::z80RlReg,                   // 00010101: RL L
    &Z80::z80RlPtrHl,                 // 00010110: RL (HL)
    &Z80::z80RlReg,                   // 00010111: RL A
    &Z80::z80RrReg,                   // 00011000: RR B
    &Z80::z80RrReg,                   // 00011001: RR C
    &Z80::z80RrReg,                   // 00011010: RR D
    &Z80::z80RrReg,                   // 00011011: RR E
    &Z80::z80RrReg,                   // 00011100: RR H
    &Z80::z80RrReg,                   // 00011101: RR L
    &Z80::z80RrPtrHl,                 // 00011110: RR (HL)
    &Z80::z80RrReg,                   // 00011111: RR A
    &Z80::z80SlaReg,                  // 00100000: SLA B
    &Z80::z80SlaReg,                  // 00100001: SLA C
    &Z80::z80SlaReg,                  // 00100010: SLA D
    &Z80::z80SlaReg,                  // 00100011: SLA E
    &Z80::z80SlaReg,                  // 00100100: SLA H
    &Z80::z80SlaReg,                  // 00100101: SLA L
    &Z80::z80SlaPtrHl,                // 00100110: SLA (HL)
    &Z80::z80SlaReg,                  // 00100111: SLA A
    &Z80::z80SraReg,                  // 00101000: SRA B
    &Z80::z80SraReg,                  // 00101001: SRA C
    &Z80::z80SraReg,                  // 00101010: SRA D
    &Z80::z80SraReg,                  // 00101011: SRA E
    &Z80::z80SraReg,                  // 00101100: SRA H
    &Z80::z80SraReg,                  // 00101101: SRA L
    &Z80::z80SraPtrHl,                // 00101110: SRA (HL)
    &Z80::z80SraReg,                  // 00101111: SRA A
    &Z80::z80SllReg,                  // 00110000: SLL B
    &Z80::z80SllReg,                  // 00110001: SLL C
    &Z80::z80SllReg,                  // 00110010: SLL D
    &Z80::z80SllReg,                  // 00110011: SLL E
    &Z80::z80SllReg,                  // 00110100: SLL H
    &Z80::z80SllReg,                  // 00100101: SLL L
    &Z80::z80SllPtrHl,                // 00100110: SLL (HL)
    &Z80::z80SllReg,                  // 00100111: SLL A
    &Z80::z80SrlReg,                  // 00111000: SRL B
    &Z80::z80SrlReg,                  // 00111001: SRL C
    &Z80::z80SrlReg,                  // 00111010: SRL D
    &Z80::z80SrlReg,                  // 00111011: SRL E
    &Z80::z80SrlReg,                  // 00111100: SRL H
    &Z80::z80SrlReg,                  // 00111101: SRL L
    &Z80::z80SrlPtrHl,                // 00111110: SRL (HL)
    &Z80::z80SrlReg,                  // 00111111: SRL A
    &Z80::z80BitNReg,                 // 01000000: BIT 0, B
    &Z80::z80BitNReg,                 // 01000001: BIT 0, C
    &Z80::z80BitNReg,                 // 01000010: BIT 0, D
    &Z80::z80BitNReg,                 // 01000011: BIT 0, E
    &Z80::z80BitNReg,                 // 01000100: BIT 0, H
    &Z80::z80BitNReg,                 // 01000101: BIT 0, L
    &Z80::z80BitNPtrHl,               // 01000110: BIT 0, (HL)
    &Z80::z80BitNReg,                 // 01000111: BIT 0, A
    &Z80::z80BitNReg,                 // 01001000: BIT 1, B
    &Z80::z80BitNReg,                 // 01001001: BIT 1, C
    &Z80::z80BitNReg,                 // 01001010: BIT 1, D
    &Z80::z80BitNReg,                 // 01001011: BIT 1, E
    &Z80::z80BitNReg,                 // 01001100: BIT 1, H
    &Z80::z80BitNReg,                 // 01001101: BIT 1, L
    &Z80::z80BitNPtrHl,               // 01001110: BIT 1, (HL)
    &Z80::z80BitNReg,                 // 01001111: BIT 1, A
    &Z80::z80BitNReg,                 // 01010000: BIT 2, B
    &Z80::z80BitNReg,                 // 01010001: BIT 2, C
    &Z80::z80BitNReg,                 // 01010010: BIT 2, D
    &Z80::z80BitNReg,                 // 01010011: BIT 2, E
    &Z80::z80BitNReg,                 // 01010100: BIT 2, H
    &Z80::z80BitNReg,                 // 01010101: BIT 2, L
    &Z80::z80BitNPtrHl,               // 01010110: BIT 2, (HL)
    &Z80::z80BitNReg,                 // 01010111: BIT 2, A
    &Z80::z80BitNReg,                 // 01011000: BIT 3, B
    &Z80::z80BitNReg,                 // 01011001: BIT 3, C
    &Z80::z80BitNReg,                 // 01011010: BIT 3, D
    &Z80::z80BitNReg,                 // 01011011: BIT 3, E
    &Z80::z80BitNReg,                 // 01011100: BIT 3, H
    &Z80::z80BitNReg,                 // 01011101: BIT 3, L
    &Z80::z80BitNPtrHl,               // 01011110: BIT 3, (HL)
    &Z80::z80BitNReg,                 // 01011111: BIT 3, A
    &Z80::z80BitNReg,                 // 01100000: BIT 4, B
    &Z80::z80BitNReg,                 // 01100001: BIT 4, C
    &Z80::z80BitNReg,                 // 01100010: BIT 4, D
    &Z80::z80BitNReg,                 // 01100011: BIT 4, E
    &Z80::z80BitNReg,                 // 01100100: BIT 4, H
    &Z80::z80BitNReg,                 // 01100101: BIT 4, L
    &Z80::z80BitNPtrHl,               // 01100110: BIT 4, (HL)
    &Z80::z80BitNReg,                 // 01100111: BIT 4, A
    &Z80::z80BitNReg,                 // 01101000: BIT 5, B
    &Z80::z80BitNReg,                 // 01101001: BIT 5, C
    &Z80::z80BitNReg,                 // 01101010: BIT 5, D
    &Z80::z80BitNReg,                 // 01101011: BIT 5, E
    &Z80::z80BitNReg,                 // 01101100: BIT 5, H
    &Z80::z80BitNReg,                 // 01101101: BIT 5, L
    &Z80::z80BitNPtrHl,               // 01101110: BIT 5, (HL)
    &Z80::z80BitNReg,                 // 01101111: BIT 5, A
    &Z80::z80BitNReg,                 // 01110000: BIT 6, B
    &Z80::z80BitNReg,                 // 01110001: BIT 6, C
    &Z80::z80BitNReg,                 // 01110010: BIT 6, D
    &Z80::z80BitNReg,                 // 01110011: BIT 6, E
    &Z80::z80BitNReg,                 // 01110100: BIT 6, H
    &Z80::z80BitNReg,                 // 01110101: BIT 6, L
    &Z80::z80BitNPtrHl,               // 01110110: BIT 6, (HL)
    &Z80::z80BitNReg,                 // 01110111: BIT 6, A
    &Z80::z80BitNReg,                 // 01111000: BIT 7, B
    &Z80::z80BitNReg,                 // 01111001: BIT 7, C
    &Z80::z80BitNReg,                 // 01111010: BIT 7, D
    &Z80::z80BitNReg,                 // 01111011: BIT 7, E
    &Z80::z80BitNReg,                 // 01111100: BIT 7, H
    &Z80::z80BitNReg,                 // 01111101: BIT 7, L
    &Z80::z80BitNPtrHl,               // 01111110: BIT 7, (HL)
    &Z80::z80BitNReg,                 // 01111111: BIT 7, A
    &Z80::z80ResNReg,                 // 10000000: RES 0, B
    &Z80::z80ResNReg,                 // 10000001: RES 0, C
    &Z80::z80ResNReg,                 // 10000010: RES 0, D
    &Z80::z80ResNReg,                 // 10000011: RES 0, E
    &Z80::z80ResNReg,                 // 10000100: RES 0, H
    &Z80::z80ResNReg,                 // 10000101: RES 0, L
    &Z80::z80ResNPtrHl,               // 10000110: RES 0, (HL)
    &Z80::z80ResNReg,                 // 10000111: RES 0, A
    &Z80::z80ResNReg,                 // 10001000: RES 1, B
    &Z80::z80ResNReg,                 // 10001001: RES 1, C
    &Z80::z80ResNReg,                 // 10001010: RES 1, D
    &Z80::z80ResNReg,                 // 10001011: RES 1, E
    &Z80::z80ResNReg,                 // 10001100: RES 1, H
    &Z80::z80ResNReg,                 // 10001101: RES 1, L
    &Z80::z80ResNPtrHl,               // 10001110: RES 1, (HL)
    &Z80::z80ResNReg,                 // 10001111: RES 1, A
    &Z80::z80ResNReg,                 // 10010000: RES 2, B
    &Z80::z80ResNReg,                 // 10010001: RES 2, C
    &Z80::z80ResNReg,                 // 10010010: RES 2, D
    &Z80::z80ResNReg,                 // 10010011: RES 2, E
    &Z80::z80ResNReg,                 // 10010100: RES 2, H
    &Z80::z80ResNReg,                 // 10010101: RES 2, L
    &Z80::z80ResNPtrHl,               // 10010110: RES 2, (HL)
    &Z80::z80ResNReg,                 // 10010111: RES 2, A
    &Z80::z80ResNReg,                 // 10011000: RES 3, B
    &Z80::z80ResNReg,                 // 10011001: RES 3, C
    &Z80::z80ResNReg,                 // 10011010: RES 3, D
    &Z80::z80ResNReg,                 // 10011011: RES 3, E
    &Z80::z80ResNReg,                 // 10011100: RES 3, H
    &Z80::z80ResNReg,                 // 10011101: RES 3, L
    &Z80::z80ResNPtrHl,               // 10011110: RES 3, (HL)
    &Z80::z80ResNReg,                 // 10011111: RES 3, A
    &Z80::z80ResNReg,                 // 10100000: RES 4, B
    &Z80::z80ResNReg,                 // 10100001: RES 4, C
    &Z80::z80ResNReg,                 // 10100010: RES 4, D
    &Z80::z80ResNReg,                 // 10100011: RES 4, E
    &Z80::z80ResNReg,                 // 10100100: RES 4, H
    &Z80::z80ResNReg,                 // 10100101: RES 4, L
    &Z80::z80ResNPtrHl,               // 10100110: RES 4, (HL)
    &Z80::z80ResNReg,                 // 10100111: RES 4, A
    &Z80::z80ResNReg,                 // 10101000: RES 5, B
    &Z80::z80ResNReg,                 // 10101001: RES 5, C
    &Z80::z80ResNReg,                 // 10101010: RES 5, D
    &Z80::z80ResNReg,                 // 10101011: RES 5, E
    &Z80::z80ResNReg,                 // 10101100: RES 5, H
    &Z80::z80ResNReg,                 // 10101101: RES 5, L
    &Z80::z80ResNPtrHl,               // 10101110: RES 5, (HL)
    &Z80::z80ResNReg,                 // 10101111: RES 5, A
    &Z80::z80ResNReg,                 // 10110000: RES 6, B
    &Z80::z80ResNReg,                 // 10110001: RES 6, C
    &Z80::z80ResNReg,                 // 10110010: RES 6, D
    &Z80::z80ResNReg,                 // 10110011: RES 6, E
    &Z80::z80ResNReg,                 // 10110100: RES 6, H
    &Z80::z80ResNReg,                 // 10110101: RES 6, L
    &Z80::z80ResNPtrHl,               // 10110110: RES 6, (HL)
    &Z80::z80ResNReg,                 // 10110111: RES 6, A
    &Z80::z80ResNReg,                 // 10111000: RES 7, B
    &Z80::z80ResNReg,                 // 10111001: RES 7, C
    &Z80::z80ResNReg,                 // 10111010: RES 7, D
    &Z80::z80ResNReg,                 // 10111011: RES 7, E
    &Z80::z80ResNReg,                 // 10111100: RES 7, H
    &Z80::z80ResNReg,                 // 10111101: RES 7, L
    &Z80::z80ResNPtrHl,               // 10111110: RES 7, (HL)
    &Z80::z80ResNReg,                 // 10111111: RES 7, A
    &Z80::z80SetNReg,                 // 11000000: SET 0, B
    &Z80::z80SetNReg,                 // 11000001: SET 0, C
    &Z80::z80SetNReg,                 // 11000010: SET 0, D
    &Z80::z80SetNReg,                 // 11000011: SET 0, E
    &Z80::z80SetNReg,                 // 11000100: SET 0, H
    &Z80::z80SetNReg,                 // 11000101: SET 0, L
    &Z80::z80SetNPtrHl,               // 11000110: SET 0, (HL)
    &Z80::z80SetNReg,                 // 11000111: SET 0, A
    &Z80::z80SetNReg,                 // 11001000: SET 1, B
    &Z80::z80SetNReg,                 // 11001001: SET 1, C
    &Z80::z80SetNReg,                 // 11001010: SET 1, D
    &Z80::z80SetNReg,                 // 11001011: SET 1, E
    &Z80::z80SetNReg,                 // 11001100: SET 1, H
    &Z80::z80SetNReg,                 // 11001101: SET 1, L
    &Z80::z80SetNPtrHl,               // 11001110: SET 1, (HL)
    &Z80::z80SetNReg,                 // 11001111: SET 1, A
    &Z80::z80SetNReg,                 // 11010000: SET 2, B
    &Z80::z80SetNReg,                 // 11010001: SET 2, C
    &Z80::z80SetNReg,                 // 11010010: SET 2, D
    &Z80::z80SetNReg,                 // 11010011: SET 2, E
    &Z80::z80SetNReg,                 // 11010100: SET 2, H
    &Z80::z80SetNReg,                 // 11010101: SET 2, L
    &Z80::z80SetNPtrHl,               // 11010110: SET 2, (HL)
    &Z80::z80SetNReg,                 // 11010111: SET 2, A
    &Z80::z80SetNReg,                 // 11011000: SET 3, B
    &Z80::z80SetNReg,                 // 11011001: SET 3, C
    &Z80::z80SetNReg,                 // 11011010: SET 3, D
    &Z80::z80SetNReg,                 // 11011011: SET 3, E
    &Z80::z80SetNReg,                 // 11011100: SET 3, H
    &Z80::z80SetNReg,                 // 11011101: SET 3, L
    &Z80::z80SetNPtrHl,               // 11011110: SET 3, (HL)
    &Z80::z80SetNReg,                 // 11011111: SET 3, A
    &Z80::z80SetNReg,                 // 11100000: SET 4, B
    &Z80::z80SetNReg,                 // 11100001: SET 4, C
    &Z80::z80SetNReg,                 // 11100010: SET 4, D
    &Z80::z80SetNReg,                 // 11100011: SET 4, E
    &Z80::z80SetNReg,                 // 11100100: SET 4, H
    &Z80::z80SetNReg,                 // 11100101: SET 4, L
    &Z80::z80SetNPtrHl,               // 11100110: SET 4, (HL)
    &Z80::z80SetNReg,                 // 11100111: SET 4, A
    &Z80::z80SetNReg,                 // 11101000: SET 5, B
    &Z80::z80SetNReg,                 // 11101001: SET 5, C
    &Z80::z80SetNReg,                 // 11101010: SET 5, D
    &Z80::z80SetNReg,                 // 11101011: SET 5, E
    &Z80::z80SetNReg,                 // 11101100: SET 5, H
    &Z80::z80SetNReg,                 // 11101101: SET 5, L
    &Z80::z80SetNPtrHl,               // 11101110: SET 5, (HL)
    &Z80::z80SetNReg,                 // 11101111: SET 5, A
    &Z80::z80SetNReg,                 // 11110000: SET 6, B
    &Z80::z80SetNReg,                 // 11110001: SET 6, C
    &Z80::z80SetNReg,                 // 11110010: SET 6, D
    &Z80::z80SetNReg,                 // 11110011: SET 6, E
    &Z80::z80SetNReg,                 // 11110100: SET 6, H
    &Z80::z80SetNReg,                 // 11110101: SET 6, L
    &Z80::z80SetNPtrHl,               // 11110110: SET 6, (HL)
    &Z80::z80SetNReg,                 // 11110111: SET 6, A
    &Z80::z80SetNReg,                 // 11111000: SET 7, B
    &Z80::z80SetNReg,                 // 11111001: SET 7, C
    &Z80::z80SetNReg,                 // 11111010: SET 7, D
    &Z80::z80SetNReg,                 // 11111011: SET 7, E
    &Z80::z80SetNReg,                 // 11111100: SET 7, H
    &Z80::z80SetNReg,                 // 11111101: SET 7, L
    &Z80::z80SetNPtrHl,               // 11111110: SET 7, (HL)
    &Z80::z80SetNReg                  // 11111111: SET 7, A
},
