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

xxcbprefixed
{
    &Z80::z80RlcPtrIxIy,              // 00000000: RLC (In + d), B
    &Z80::z80RlcPtrIxIy,              // 00000001: RLC (In + d), C
    &Z80::z80RlcPtrIxIy,              // 00000010: RLC (In + d), D
    &Z80::z80RlcPtrIxIy,              // 00000011: RLC (In + d), E
    &Z80::z80RlcPtrIxIy,              // 00000100: RLC (In + d), H
    &Z80::z80RlcPtrIxIy,              // 00000101: RLC (In + d), L
    &Z80::z80RlcPtrIxIy,              // 00000110: RLC (In + d)
    &Z80::z80RlcPtrIxIy,              // 00000111: RLC (In + d), A
    &Z80::z80RrcPtrIxIy,              // 00001000: RRC (In + d), B
    &Z80::z80RrcPtrIxIy,              // 00001001: RRC (In + d), C
    &Z80::z80RrcPtrIxIy,              // 00001010: RRC (In + d), D
    &Z80::z80RrcPtrIxIy,              // 00001011: RRC (In + d), E
    &Z80::z80RrcPtrIxIy,              // 00001100: RRC (In + d), H
    &Z80::z80RrcPtrIxIy,              // 00001101: RRC (In + d), L
    &Z80::z80RrcPtrIxIy,              // 00001110: RRC (In + d)
    &Z80::z80RrcPtrIxIy,              // 00001111: RRC (In + d), A
    &Z80::z80RlPtrIxIy,               // 00010000: RL (In + d), B
    &Z80::z80RlPtrIxIy,               // 00010001: RL (In + d), C
    &Z80::z80RlPtrIxIy,               // 00010010: RL (In + d), D
    &Z80::z80RlPtrIxIy,               // 00010011: RL (In + d), E
    &Z80::z80RlPtrIxIy,               // 00010100: RL (In + d), H
    &Z80::z80RlPtrIxIy,               // 00010101: RL (In + d), L
    &Z80::z80RlPtrIxIy,               // 00010110: RL (In + d)
    &Z80::z80RlPtrIxIy,               // 00010111: RL (In + d), A
    &Z80::z80RrPtrIxIy,               // 00011000: RR (In + d), B
    &Z80::z80RrPtrIxIy,               // 00011001: RR (In + d), C
    &Z80::z80RrPtrIxIy,               // 00011010: RR (In + d), D
    &Z80::z80RrPtrIxIy,               // 00011011: RR (In + d), E
    &Z80::z80RrPtrIxIy,               // 00011100: RR (In + d), H
    &Z80::z80RrPtrIxIy,               // 00011101: RR (In + d), L
    &Z80::z80RrPtrIxIy,               // 00011110: RR (In + d)
    &Z80::z80RrPtrIxIy,               // 00011111: RR (In + d), A
    &Z80::z80SlaPtrIxIy,              // 00100000: SLA (In + d), B
    &Z80::z80SlaPtrIxIy,              // 00100001: SLA (In + d), C
    &Z80::z80SlaPtrIxIy,              // 00100010: SLA (In + d), D
    &Z80::z80SlaPtrIxIy,              // 00100011: SLA (In + d), E
    &Z80::z80SlaPtrIxIy,              // 00100100: SLA (In + d), H
    &Z80::z80SlaPtrIxIy,              // 00100101: SLA (In + d), L
    &Z80::z80SlaPtrIxIy,              // 00100110: SLA (In + d)
    &Z80::z80SlaPtrIxIy,              // 00100111: SLA (In + d), A
    &Z80::z80SraPtrIxIy,              // 00101000: SRA (In + d), B
    &Z80::z80SraPtrIxIy,              // 00101001: SRA (In + d), C
    &Z80::z80SraPtrIxIy,              // 00101010: SRA (In + d), D
    &Z80::z80SraPtrIxIy,              // 00101011: SRA (In + d), E
    &Z80::z80SraPtrIxIy,              // 00101100: SRA (In + d), H
    &Z80::z80SraPtrIxIy,              // 00101101: SRA (In + d), L
    &Z80::z80SraPtrIxIy,              // 00101110: SRA (In + d)
    &Z80::z80SraPtrIxIy,              // 00101111: SRA (In + d), A
    &Z80::z80SllPtrIxIy,              // 00110000: SLL (In + d), B
    &Z80::z80SllPtrIxIy,              // 00110001: SLL (In + d), C
    &Z80::z80SllPtrIxIy,              // 00110010: SLL (In + d), D
    &Z80::z80SllPtrIxIy,              // 00110011: SLL (In + d), E
    &Z80::z80SllPtrIxIy,              // 00110100: SLL (In + d), H
    &Z80::z80SllPtrIxIy,              // 00110101: SLL (In + d), L
    &Z80::z80SllPtrIxIy,              // 00110110: SLL (In + d)
    &Z80::z80SllPtrIxIy,              // 00110111: SLL (In + d), A
    &Z80::z80SrlPtrIxIy,              // 00111000: SRL (In + d), B
    &Z80::z80SrlPtrIxIy,              // 00111001: SRL (In + d), C
    &Z80::z80SrlPtrIxIy,              // 00111010: SRL (In + d), D
    &Z80::z80SrlPtrIxIy,              // 00111011: SRL (In + d), E
    &Z80::z80SrlPtrIxIy,              // 00111100: SRL (In + d), H
    &Z80::z80SrlPtrIxIy,              // 00111101: SRL (In + d), L
    &Z80::z80SrlPtrIxIy,              // 00111110: SRL (In + d)
    &Z80::z80SrlPtrIxIy,              // 00111111: SRL (In + d), A
    &Z80::z80BitNPtrIxIy,             // 01000000: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000001: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000010: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000011: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000100: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000101: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000110: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01000111: BIT 0, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001000: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001001: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001010: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001011: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001100: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001101: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001110: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01001111: BIT 1, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010000: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010001: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010010: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010011: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010100: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010101: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010110: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01010111: BIT 2, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011000: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011001: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011010: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011011: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011100: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011101: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011110: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01011111: BIT 3, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100000: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100001: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100010: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100011: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100100: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100101: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100110: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01100111: BIT 4, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101000: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101001: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101010: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101011: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101100: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101101: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101110: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01101111: BIT 5, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110000: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110001: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110010: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110011: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110100: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110101: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110110: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01110111: BIT 6, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111000: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111001: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111010: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111011: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111100: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111101: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111110: BIT 7, (In + d)
    &Z80::z80BitNPtrIxIy,             // 01111111: BIT 7, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10000000: RES 0, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10000001: RES 0, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10000010: RES 0, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10000011: RES 0, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10000100: RES 0, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10000101: RES 0, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10000110: RES 0, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10000111: RES 0, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10001000: RES 1, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10001001: RES 1, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10001010: RES 1, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10001011: RES 1, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10001100: RES 1, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10001101: RES 1, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10001110: RES 1, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10001111: RES 1, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10010000: RES 2, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10010001: RES 2, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10010010: RES 2, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10010011: RES 2, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10010100: RES 2, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10010101: RES 2, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10010110: RES 2, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10010111: RES 2, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10011000: RES 3, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10011001: RES 3, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10011010: RES 3, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10011011: RES 3, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10011100: RES 3, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10011101: RES 3, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10011110: RES 3, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10011111: RES 3, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10100000: RES 4, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10100001: RES 4, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10100010: RES 4, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10100011: RES 4, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10100100: RES 4, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10100101: RES 4, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10100110: RES 4, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10100111: RES 4, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10101000: RES 5, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10101001: RES 5, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10101010: RES 5, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10101011: RES 5, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10101100: RES 5, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10101101: RES 5, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10101110: RES 5, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10101111: RES 5, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10110000: RES 6, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10110001: RES 6, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10110010: RES 6, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10110011: RES 6, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10110100: RES 6, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10110101: RES 6, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10110110: RES 6, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10110111: RES 6, (In + d), A
    &Z80::z80ResNPtrIxIy,             // 10111000: RES 7, (In + d), B
    &Z80::z80ResNPtrIxIy,             // 10111001: RES 7, (In + d), C
    &Z80::z80ResNPtrIxIy,             // 10111010: RES 7, (In + d), D
    &Z80::z80ResNPtrIxIy,             // 10111011: RES 7, (In + d), E
    &Z80::z80ResNPtrIxIy,             // 10111100: RES 7, (In + d), H
    &Z80::z80ResNPtrIxIy,             // 10111101: RES 7, (In + d), L
    &Z80::z80ResNPtrIxIy,             // 10111110: RES 7, (In + d)
    &Z80::z80ResNPtrIxIy,             // 10111111: RES 7, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11000000: SET 0, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11000001: SET 0, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11000010: SET 0, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11000011: SET 0, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11000100: SET 0, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11000101: SET 0, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11000110: SET 0, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11000111: SET 0, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11001000: SET 1, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11001001: SET 1, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11001010: SET 1, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11001011: SET 1, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11001100: SET 1, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11001101: SET 1, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11001110: SET 1, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11001111: SET 1, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11010000: SET 2, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11010001: SET 2, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11010010: SET 2, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11010011: SET 2, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11010100: SET 2, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11010101: SET 2, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11010110: SET 2, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11010111: SET 2, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11011000: SET 3, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11011001: SET 3, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11011010: SET 3, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11011011: SET 3, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11011100: SET 3, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11011101: SET 3, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11011110: SET 3, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11011111: SET 3, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11100000: SET 4, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11100001: SET 4, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11100010: SET 4, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11100011: SET 4, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11100100: SET 4, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11100101: SET 4, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11100110: SET 4, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11100111: SET 4, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11101000: SET 5, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11101001: SET 5, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11101010: SET 5, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11101011: SET 5, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11101100: SET 5, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11101101: SET 5, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11101110: SET 5, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11101111: SET 5, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11110000: SET 6, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11110001: SET 6, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11110010: SET 6, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11110011: SET 6, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11110100: SET 6, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11110101: SET 6, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11110110: SET 6, (In + d)
    &Z80::z80SetNPtrIxIy,             // 11110111: SET 6, (In + d), A
    &Z80::z80SetNPtrIxIy,             // 11111000: SET 7, (In + d), B
    &Z80::z80SetNPtrIxIy,             // 11111001: SET 7, (In + d), C
    &Z80::z80SetNPtrIxIy,             // 11111010: SET 7, (In + d), D
    &Z80::z80SetNPtrIxIy,             // 11111011: SET 7, (In + d), E
    &Z80::z80SetNPtrIxIy,             // 11111100: SET 7, (In + d), H
    &Z80::z80SetNPtrIxIy,             // 11111101: SET 7, (In + d), L
    &Z80::z80SetNPtrIxIy,             // 11111110: SET 7, (In + d)
    &Z80::z80SetNPtrIxIy             // 11111111: SET 7, (In + d), A
},
