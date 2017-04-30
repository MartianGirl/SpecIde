#pragma once

/** Z80XXCBPrefixed.h
 *
 * Z80 instruction table for DDCB and FDCB-prefixed opcodes.
 * The type of addressing is determined by the prefix.
 *
 */

#include "Z80Instruction.h"
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

class Z80XXCBPrefixed
{
    public:
        // Instructions
        Z80Nop iNop; 

        Z80RlcPtrIxIy iRlcPtrIxIy;
        Z80RrcPtrIxIy iRrcPtrIxIy;
        Z80RlPtrIxIy iRlPtrIxIy;
        Z80RrPtrIxIy iRrPtrIxIy;
        Z80SlaPtrIxIy iSlaPtrIxIy;
        Z80SraPtrIxIy iSraPtrIxIy;
        Z80SllPtrIxIy iSllPtrIxIy;
        Z80SrlPtrIxIy iSrlPtrIxIy;

        Z80BitNPtrIxIy iBitNPtrIxIy;
        Z80ResNPtrIxIy iResNPtrIxIy;
        Z80SetNPtrIxIy iSetNPtrIxIy;

        Z80Instruction* table[4][8][8];

        Z80XXCBPrefixed() :
            table {
                // x = 0
                {
                    // y = 0
                    {
                        &iRlcPtrIxIy,       // 00000000: RLC (In + d), B
                        &iRlcPtrIxIy,       // 00000001: RLC (In + d), C
                        &iRlcPtrIxIy,       // 00000010: RLC (In + d), D
                        &iRlcPtrIxIy,       // 00000011: RLC (In + d), E
                        &iRlcPtrIxIy,       // 00000100: RLC (In + d), H
                        &iRlcPtrIxIy,       // 00000101: RLC (In + d), L
                        &iRlcPtrIxIy,       // 00000110: RLC (In + d)
                        &iRlcPtrIxIy        // 00000111: RLC (In + d), A
                    },
                    // y = 1
                    {
                        &iRrcPtrIxIy,       // 00001000: RRC (In + d), B
                        &iRrcPtrIxIy,       // 00001001: RRC (In + d), C
                        &iRrcPtrIxIy,       // 00001010: RRC (In + d), D
                        &iRrcPtrIxIy,       // 00001011: RRC (In + d), E
                        &iRrcPtrIxIy,       // 00001100: RRC (In + d), H
                        &iRrcPtrIxIy,       // 00001101: RRC (In + d), L
                        &iRrcPtrIxIy,       // 00001110: RRC (In + d)
                        &iRrcPtrIxIy        // 00001111: RRC (In + d), A
                    },
                    // y = 2
                    {
                        &iRlPtrIxIy,       // 00010000: RL (In + d), B
                        &iRlPtrIxIy,       // 00010001: RL (In + d), C
                        &iRlPtrIxIy,       // 00010010: RL (In + d), D
                        &iRlPtrIxIy,       // 00010011: RL (In + d), E
                        &iRlPtrIxIy,       // 00010100: RL (In + d), H
                        &iRlPtrIxIy,       // 00010101: RL (In + d), L
                        &iRlPtrIxIy,       // 00010110: RL (In + d)
                        &iRlPtrIxIy        // 00010111: RL (In + d), A
                    },
                    // y = 3
                    {
                        &iRrPtrIxIy,       // 00011000: RR (In + d), B
                        &iRrPtrIxIy,       // 00011001: RR (In + d), C
                        &iRrPtrIxIy,       // 00011010: RR (In + d), D
                        &iRrPtrIxIy,       // 00011011: RR (In + d), E
                        &iRrPtrIxIy,       // 00011100: RR (In + d), H
                        &iRrPtrIxIy,       // 00011101: RR (In + d), L
                        &iRrPtrIxIy,       // 00011110: RR (In + d)
                        &iRrPtrIxIy        // 00011111: RR (In + d), A
                    },
                    // y = 4
                    {
                        &iSlaPtrIxIy,       // 00100000: SLA (In + d), B
                        &iSlaPtrIxIy,       // 00100001: SLA (In + d), C
                        &iSlaPtrIxIy,       // 00100010: SLA (In + d), D
                        &iSlaPtrIxIy,       // 00100011: SLA (In + d), E
                        &iSlaPtrIxIy,       // 00100100: SLA (In + d), H
                        &iSlaPtrIxIy,       // 00100101: SLA (In + d), L
                        &iSlaPtrIxIy,       // 00100110: SLA (In + d)
                        &iSlaPtrIxIy        // 00100111: SLA (In + d), A
                    },
                    // y = 5
                    {
                        &iSraPtrIxIy,       // 00101000: SRA (In + d), B
                        &iSraPtrIxIy,       // 00101001: SRA (In + d), C
                        &iSraPtrIxIy,       // 00101010: SRA (In + d), D
                        &iSraPtrIxIy,       // 00101011: SRA (In + d), E
                        &iSraPtrIxIy,       // 00101100: SRA (In + d), H
                        &iSraPtrIxIy,       // 00101101: SRA (In + d), L
                        &iSraPtrIxIy,       // 00101110: SRA (In + d)
                        &iSraPtrIxIy        // 00101111: SRA (In + d), A
                    },
                    // y = 6
                    {
                        &iSllPtrIxIy,       // 00110000: SLL (In + d), B
                        &iSllPtrIxIy,       // 00110001: SLL (In + d), C
                        &iSllPtrIxIy,       // 00110010: SLL (In + d), D
                        &iSllPtrIxIy,       // 00110011: SLL (In + d), E
                        &iSllPtrIxIy,       // 00110100: SLL (In + d), H
                        &iSllPtrIxIy,       // 00110101: SLL (In + d), L
                        &iSllPtrIxIy,       // 00110110: SLL (In + d)
                        &iSllPtrIxIy        // 00110111: SLL (In + d), A
                    },
                    // y = 7
                    {
                        &iSrlPtrIxIy,       // 00111000: SRL (In + d), B
                        &iSrlPtrIxIy,       // 00111001: SRL (In + d), C
                        &iSrlPtrIxIy,       // 00111010: SRL (In + d), D
                        &iSrlPtrIxIy,       // 00111011: SRL (In + d), E
                        &iSrlPtrIxIy,       // 00111100: SRL (In + d), H
                        &iSrlPtrIxIy,       // 00111101: SRL (In + d), L
                        &iSrlPtrIxIy,       // 00111110: SRL (In + d)
                        &iSrlPtrIxIy        // 00111111: SRL (In + d), A
                    }
                },
                // x = 1
                {
                    // y = 0
                    {
                        &iBitNPtrIxIy,      // 01000000: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000001: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000010: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000011: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000100: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000101: BIT 0, (In + d)
                        &iBitNPtrIxIy,      // 01000110: BIT 0, (In + d)
                        &iBitNPtrIxIy       // 01000111: BIT 0, (In + d)
                    },
                    // y = 1
                    {
                        &iBitNPtrIxIy,      // 01001000: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001001: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001010: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001011: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001100: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001101: BIT 1, (In + d)
                        &iBitNPtrIxIy,      // 01001110: BIT 1, (In + d)
                        &iBitNPtrIxIy       // 01001111: BIT 1, (In + d)
                    },
                    // y = 2
                    {
                        &iBitNPtrIxIy,      // 01010000: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010001: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010010: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010011: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010100: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010101: BIT 2, (In + d)
                        &iBitNPtrIxIy,      // 01010110: BIT 2, (In + d)
                        &iBitNPtrIxIy       // 01010111: BIT 2, (In + d)
                    },
                    // y = 3
                    {
                        &iBitNPtrIxIy,      // 01011000: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011001: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011010: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011011: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011100: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011101: BIT 3, (In + d)
                        &iBitNPtrIxIy,      // 01011110: BIT 3, (In + d)
                        &iBitNPtrIxIy       // 01011111: BIT 3, (In + d)
                    },
                    // y = 4
                    {
                        &iBitNPtrIxIy,      // 01100000: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100001: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100010: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100011: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100100: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100101: BIT 4, (In + d)
                        &iBitNPtrIxIy,      // 01100110: BIT 4, (In + d)
                        &iBitNPtrIxIy       // 01100111: BIT 4, (In + d)
                    },
                    // y = 5
                    {
                        &iBitNPtrIxIy,      // 01101000: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101001: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101010: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101011: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101100: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101101: BIT 5, (In + d)
                        &iBitNPtrIxIy,      // 01101110: BIT 5, (In + d)
                        &iBitNPtrIxIy       // 01101111: BIT 5, (In + d)
                    },
                    // y = 6
                    {
                        &iBitNPtrIxIy,      // 01110000: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110001: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110010: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110011: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110100: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110101: BIT 6, (In + d)
                        &iBitNPtrIxIy,      // 01110110: BIT 6, (In + d)
                        &iBitNPtrIxIy       // 01110111: BIT 6, (In + d)
                    },
                    // y = 7
                    {
                        &iBitNPtrIxIy,      // 01111000: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111001: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111010: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111011: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111100: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111101: BIT 7, (In + d)
                        &iBitNPtrIxIy,      // 01111110: BIT 7, (In + d)
                        &iBitNPtrIxIy       // 01111111: BIT 7, (In + d)
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iResNPtrIxIy,      // 10000000: RES 0, (In + d), B
                        &iResNPtrIxIy,      // 10000001: RES 0, (In + d), C
                        &iResNPtrIxIy,      // 10000010: RES 0, (In + d), D
                        &iResNPtrIxIy,      // 10000011: RES 0, (In + d), E
                        &iResNPtrIxIy,      // 10000100: RES 0, (In + d), H
                        &iResNPtrIxIy,      // 10000101: RES 0, (In + d), L
                        &iResNPtrIxIy,      // 10000110: RES 0, (In + d)
                        &iResNPtrIxIy       // 10000111: RES 0, (In + d), A
                    },
                    // y = 1
                    {
                        &iResNPtrIxIy,      // 10001000: RES 1, (In + d), B
                        &iResNPtrIxIy,      // 10001001: RES 1, (In + d), C
                        &iResNPtrIxIy,      // 10001010: RES 1, (In + d), D
                        &iResNPtrIxIy,      // 10001011: RES 1, (In + d), E
                        &iResNPtrIxIy,      // 10001100: RES 1, (In + d), H
                        &iResNPtrIxIy,      // 10001101: RES 1, (In + d), L
                        &iResNPtrIxIy,      // 10001110: RES 1, (In + d)
                        &iResNPtrIxIy       // 10001111: RES 1, (In + d), A
                    },
                    // y = 2
                    {
                        &iResNPtrIxIy,      // 10010000: RES 2, (In + d), B
                        &iResNPtrIxIy,      // 10010001: RES 2, (In + d), C
                        &iResNPtrIxIy,      // 10010010: RES 2, (In + d), D
                        &iResNPtrIxIy,      // 10010011: RES 2, (In + d), E
                        &iResNPtrIxIy,      // 10010100: RES 2, (In + d), H
                        &iResNPtrIxIy,      // 10010101: RES 2, (In + d), L
                        &iResNPtrIxIy,      // 10010110: RES 2, (In + d)
                        &iResNPtrIxIy       // 10010111: RES 2, (In + d), A
                    },
                    // y = 3
                    {
                        &iResNPtrIxIy,      // 10011000: RES 3, (In + d), B
                        &iResNPtrIxIy,      // 10011001: RES 3, (In + d), C
                        &iResNPtrIxIy,      // 10011010: RES 3, (In + d), D
                        &iResNPtrIxIy,      // 10011011: RES 3, (In + d), E
                        &iResNPtrIxIy,      // 10011100: RES 3, (In + d), H
                        &iResNPtrIxIy,      // 10011101: RES 3, (In + d), L
                        &iResNPtrIxIy,      // 10011110: RES 3, (In + d)
                        &iResNPtrIxIy       // 10011111: RES 3, (In + d), A
                    },
                    // y = 4
                    {
                        &iResNPtrIxIy,      // 10100000: RES 4, (In + d), B
                        &iResNPtrIxIy,      // 10100001: RES 4, (In + d), C
                        &iResNPtrIxIy,      // 10100010: RES 4, (In + d), D
                        &iResNPtrIxIy,      // 10100011: RES 4, (In + d), E
                        &iResNPtrIxIy,      // 10100100: RES 4, (In + d), H
                        &iResNPtrIxIy,      // 10100101: RES 4, (In + d), L
                        &iResNPtrIxIy,      // 10100110: RES 4, (In + d)
                        &iResNPtrIxIy       // 10100111: RES 4, (In + d), A
                    },
                    // y = 5
                    {
                        &iResNPtrIxIy,      // 10101000: RES 5, (In + d), B
                        &iResNPtrIxIy,      // 10101001: RES 5, (In + d), C
                        &iResNPtrIxIy,      // 10101010: RES 5, (In + d), D
                        &iResNPtrIxIy,      // 10101011: RES 5, (In + d), E
                        &iResNPtrIxIy,      // 10101100: RES 5, (In + d), H
                        &iResNPtrIxIy,      // 10101101: RES 5, (In + d), L
                        &iResNPtrIxIy,      // 10101110: RES 5, (In + d)
                        &iResNPtrIxIy       // 10101111: RES 5, (In + d), A
                    },
                    // y = 6
                    {
                        &iResNPtrIxIy,      // 10110000: RES 6, (In + d), B
                        &iResNPtrIxIy,      // 10110001: RES 6, (In + d), C
                        &iResNPtrIxIy,      // 10110010: RES 6, (In + d), D
                        &iResNPtrIxIy,      // 10110011: RES 6, (In + d), E
                        &iResNPtrIxIy,      // 10110100: RES 6, (In + d), H
                        &iResNPtrIxIy,      // 10110101: RES 6, (In + d), L
                        &iResNPtrIxIy,      // 10110110: RES 6, (In + d)
                        &iResNPtrIxIy       // 10110111: RES 6, (In + d), A
                    },
                    // y = 7
                    {
                        &iResNPtrIxIy,      // 10111000: RES 7, (In + d), B
                        &iResNPtrIxIy,      // 10111001: RES 7, (In + d), C
                        &iResNPtrIxIy,      // 10111010: RES 7, (In + d), D
                        &iResNPtrIxIy,      // 10111011: RES 7, (In + d), E
                        &iResNPtrIxIy,      // 10111100: RES 7, (In + d), H
                        &iResNPtrIxIy,      // 10111101: RES 7, (In + d), L
                        &iResNPtrIxIy,      // 10111110: RES 7, (In + d)
                        &iResNPtrIxIy       // 10111111: RES 7, (In + d), A
                    }
                },
                // x = 3
                {
                    // y = 0
                    {
                        &iSetNPtrIxIy,      // 11000000: SET 0, (In + d), B
                        &iSetNPtrIxIy,      // 11000001: SET 0, (In + d), C
                        &iSetNPtrIxIy,      // 11000010: SET 0, (In + d), D
                        &iSetNPtrIxIy,      // 11000011: SET 0, (In + d), E
                        &iSetNPtrIxIy,      // 11000100: SET 0, (In + d), H
                        &iSetNPtrIxIy,      // 11000101: SET 0, (In + d), L
                        &iSetNPtrIxIy,      // 11000110: SET 0, (In + d)
                        &iSetNPtrIxIy       // 11000111: SET 0, (In + d), A
                    },
                    // y = 1
                    {
                        &iSetNPtrIxIy,      // 11001000: SET 1, (In + d), B
                        &iSetNPtrIxIy,      // 11001001: SET 1, (In + d), C
                        &iSetNPtrIxIy,      // 11001010: SET 1, (In + d), D
                        &iSetNPtrIxIy,      // 11001011: SET 1, (In + d), E
                        &iSetNPtrIxIy,      // 11001100: SET 1, (In + d), H
                        &iSetNPtrIxIy,      // 11001101: SET 1, (In + d), L
                        &iSetNPtrIxIy,      // 11001110: SET 1, (In + d)
                        &iSetNPtrIxIy       // 11001111: SET 1, (In + d), A
                    },
                    // y = 2
                    {
                        &iSetNPtrIxIy,      // 11010000: SET 2, (In + d), B
                        &iSetNPtrIxIy,      // 11010001: SET 2, (In + d), C
                        &iSetNPtrIxIy,      // 11010010: SET 2, (In + d), D
                        &iSetNPtrIxIy,      // 11010011: SET 2, (In + d), E
                        &iSetNPtrIxIy,      // 11010100: SET 2, (In + d), H
                        &iSetNPtrIxIy,      // 11010101: SET 2, (In + d), L
                        &iSetNPtrIxIy,      // 11010110: SET 2, (In + d)
                        &iSetNPtrIxIy       // 11010111: SET 2, (In + d), A
                    },
                    // y = 3
                    {
                        &iSetNPtrIxIy,      // 11011000: SET 3, (In + d), B
                        &iSetNPtrIxIy,      // 11011001: SET 3, (In + d), C
                        &iSetNPtrIxIy,      // 11011010: SET 3, (In + d), D
                        &iSetNPtrIxIy,      // 11011011: SET 3, (In + d), E
                        &iSetNPtrIxIy,      // 11011100: SET 3, (In + d), H
                        &iSetNPtrIxIy,      // 11011101: SET 3, (In + d), L
                        &iSetNPtrIxIy,      // 11011110: SET 3, (In + d)
                        &iSetNPtrIxIy       // 11011111: SET 3, (In + d), A
                    },
                    // y = 4
                    {
                        &iSetNPtrIxIy,      // 11100000: SET 4, (In + d), B
                        &iSetNPtrIxIy,      // 11100001: SET 4, (In + d), C
                        &iSetNPtrIxIy,      // 11100010: SET 4, (In + d), D
                        &iSetNPtrIxIy,      // 11100011: SET 4, (In + d), E
                        &iSetNPtrIxIy,      // 11100100: SET 4, (In + d), H
                        &iSetNPtrIxIy,      // 11100101: SET 4, (In + d), L
                        &iSetNPtrIxIy,      // 11100110: SET 4, (In + d)
                        &iSetNPtrIxIy       // 11100111: SET 4, (In + d), A
                    },
                    // y = 5
                    {
                        &iSetNPtrIxIy,      // 11101000: SET 5, (In + d), B
                        &iSetNPtrIxIy,      // 11101001: SET 5, (In + d), C
                        &iSetNPtrIxIy,      // 11101010: SET 5, (In + d), D
                        &iSetNPtrIxIy,      // 11101011: SET 5, (In + d), E
                        &iSetNPtrIxIy,      // 11101100: SET 5, (In + d), H
                        &iSetNPtrIxIy,      // 11101101: SET 5, (In + d), L
                        &iSetNPtrIxIy,      // 11101110: SET 5, (In + d)
                        &iSetNPtrIxIy       // 11101111: SET 5, (In + d), A
                    },
                    // y = 6
                    {
                        &iSetNPtrIxIy,      // 11110000: SET 6, (In + d), B
                        &iSetNPtrIxIy,      // 11110001: SET 6, (In + d), C
                        &iSetNPtrIxIy,      // 11110010: SET 6, (In + d), D
                        &iSetNPtrIxIy,      // 11110011: SET 6, (In + d), E
                        &iSetNPtrIxIy,      // 11110100: SET 6, (In + d), H
                        &iSetNPtrIxIy,      // 11110101: SET 6, (In + d), L
                        &iSetNPtrIxIy,      // 11110110: SET 6, (In + d)
                        &iSetNPtrIxIy       // 11110111: SET 6, (In + d), A
                    },
                    // y = 7
                    {
                        &iSetNPtrIxIy,      // 11111000: SET 7, (In + d), B
                        &iSetNPtrIxIy,      // 11111001: SET 7, (In + d), C
                        &iSetNPtrIxIy,      // 11111010: SET 7, (In + d), D
                        &iSetNPtrIxIy,      // 11111011: SET 7, (In + d), E
                        &iSetNPtrIxIy,      // 11111100: SET 7, (In + d), H
                        &iSetNPtrIxIy,      // 11111101: SET 7, (In + d), L
                        &iSetNPtrIxIy,      // 11111110: SET 7, (In + d)
                        &iSetNPtrIxIy       // 11111111: SET 7, (In + d), A
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
