#pragma once

/** Z80CBPrefixed.h
 *
 * Z80 instruction table for CB-prefixed opcodes.
 *
 */

#include "Z80Instruction.h"
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

class Z80CBPrefixed
{
    public:
        // Instructions
        Z80Nop iNop; 

        Z80RlcReg iRlcReg;
        Z80RrcReg iRrcReg;
        Z80RlReg iRlReg;
        Z80RrReg iRrReg;
        Z80SlaReg iSlaReg;
        Z80SraReg iSraReg;
        Z80SllReg iSllReg;
        Z80SrlReg iSrlReg;

        Z80RlcPtrHl iRlcPtrHl;
        Z80RrcPtrHl iRrcPtrHl;
        Z80RlPtrHl iRlPtrHl;
        Z80RrPtrHl iRrPtrHl;
        Z80SlaPtrHl iSlaPtrHl;
        Z80SraPtrHl iSraPtrHl;
        Z80SllPtrHl iSllPtrHl;
        Z80SrlPtrHl iSrlPtrHl;

        Z80BitNReg iBitNReg;
        Z80BitNPtrHl iBitNPtrHl;
        Z80ResNReg iResNReg;
        Z80ResNPtrHl iResNPtrHl;
        Z80SetNReg iSetNReg;
        Z80SetNPtrHl iSetNPtrHl;

        Z80Instruction* table[4][8][8];

        Z80CBPrefixed() :
            table {
                // x = 0
                {
                    // y = 0
                    {
                        &iRlcReg,       // 00000000: RLC B
                        &iRlcReg,       // 00000001: RLC C
                        &iRlcReg,       // 00000010: RLC D
                        &iRlcReg,       // 00000011: RLC E
                        &iRlcReg,       // 00000100: RLC H
                        &iRlcReg,       // 00000101: RLC L
                        &iRlcPtrHl,     // 00000110: RLC (HL)
                        &iRlcReg        // 00000111: RLC A
                    },
                    // y = 1
                    {
                        &iRrcReg,       // 00001000: RRC B
                        &iRrcReg,       // 00001001: RRC C
                        &iRrcReg,       // 00001010: RRC D
                        &iRrcReg,       // 00001011: RRC E
                        &iRrcReg,       // 00001100: RRC H
                        &iRrcReg,       // 00001101: RRC L
                        &iRrcPtrHl,     // 00001110: RRC (HL)
                        &iRrcReg        // 00001111: RRC A
                    },
                    // y = 2
                    {
                        &iRlReg,        // 00010000: RL B
                        &iRlReg,        // 00010001: RL C
                        &iRlReg,        // 00010010: RL D
                        &iRlReg,        // 00010011: RL E
                        &iRlReg,        // 00010100: RL H
                        &iRlReg,        // 00010101: RL L
                        &iRlPtrHl,      // 00010110: RL (HL)
                        &iRlReg         // 00010111: RL A
                    },
                    // y = 3
                    {
                        &iRrReg,        // 00011000: RR B
                        &iRrReg,        // 00011001: RR C
                        &iRrReg,        // 00011010: RR D
                        &iRrReg,        // 00011011: RR E
                        &iRrReg,        // 00011100: RR H
                        &iRrReg,        // 00011101: RR L
                        &iRrPtrHl,      // 00011110: RR (HL)
                        &iRrReg         // 00011111: RR A
                    },
                    // y = 4
                    {
                        &iSlaReg,       // 00100000: SLA B
                        &iSlaReg,       // 00100001: SLA C
                        &iSlaReg,       // 00100010: SLA D
                        &iSlaReg,       // 00100011: SLA E
                        &iSlaReg,       // 00100100: SLA H
                        &iSlaReg,       // 00100101: SLA L
                        &iSlaPtrHl,     // 00100110: SLA (HL)
                        &iSlaReg        // 00100111: SLA A
                    },
                    // y = 5
                    {
                        &iSraReg,       // 00101000: SRA B
                        &iSraReg,       // 00101001: SRA C
                        &iSraReg,       // 00101010: SRA D
                        &iSraReg,       // 00101011: SRA E
                        &iSraReg,       // 00101100: SRA H
                        &iSraReg,       // 00101101: SRA L
                        &iSraPtrHl,     // 00101110: SRA (HL)
                        &iSraReg        // 00101111: SRA A
                    },
                    // y = 6
                    {
                        &iSllReg,       // 00110000: SLL B
                        &iSllReg,       // 00110001: SLL C
                        &iSllReg,       // 00110010: SLL D
                        &iSllReg,       // 00110011: SLL E
                        &iSllReg,       // 00110100: SLL H
                        &iSllReg,       // 00100101: SLL L
                        &iSllPtrHl,     // 00100110: SLL (HL)
                        &iSllReg        // 00100111: SLL A
                    },
                    // y = 7
                    {
                        &iSrlReg,       // 00111000: SRL B
                        &iSrlReg,       // 00111001: SRL C
                        &iSrlReg,       // 00111010: SRL D
                        &iSrlReg,       // 00111011: SRL E
                        &iSrlReg,       // 00111100: SRL H
                        &iSrlReg,       // 00111101: SRL L
                        &iSrlPtrHl,     // 00111110: SRL (HL)
                        &iSrlReg        // 00111111: SRL A
                    }
                },
                // x = 1
                {
                    // y = 0
                    {
                        &iBitNReg,      // 01000000: BIT 0, B
                        &iBitNReg,      // 01000001: BIT 0, C
                        &iBitNReg,      // 01000010: BIT 0, D
                        &iBitNReg,      // 01000011: BIT 0, E
                        &iBitNReg,      // 01000100: BIT 0, H
                        &iBitNReg,      // 01000101: BIT 0, L
                        &iBitNPtrHl,    // 01000110: BIT 0, (HL)
                        &iBitNReg       // 01000111: BIT 0, A
                    },
                    // y = 1
                    {
                        &iBitNReg,      // 01001000: BIT 1, B
                        &iBitNReg,      // 01001001: BIT 1, C
                        &iBitNReg,      // 01001010: BIT 1, D
                        &iBitNReg,      // 01001011: BIT 1, E
                        &iBitNReg,      // 01001100: BIT 1, H
                        &iBitNReg,      // 01001101: BIT 1, L
                        &iBitNPtrHl,    // 01001110: BIT 1, (HL)
                        &iBitNReg       // 01001111: BIT 1, A
                    },
                    // y = 2
                    {
                        &iBitNReg,      // 01010000: BIT 2, B
                        &iBitNReg,      // 01010001: BIT 2, C
                        &iBitNReg,      // 01010010: BIT 2, D
                        &iBitNReg,      // 01010011: BIT 2, E
                        &iBitNReg,      // 01010100: BIT 2, H
                        &iBitNReg,      // 01010101: BIT 2, L
                        &iBitNPtrHl,    // 01010110: BIT 2, (HL)
                        &iBitNReg       // 01010111: BIT 2, A
                    },
                    // y = 3
                    {
                        &iBitNReg,      // 01011000: BIT 3, B
                        &iBitNReg,      // 01011001: BIT 3, C
                        &iBitNReg,      // 01011010: BIT 3, D
                        &iBitNReg,      // 01011011: BIT 3, E
                        &iBitNReg,      // 01011100: BIT 3, H
                        &iBitNReg,      // 01011101: BIT 3, L
                        &iBitNPtrHl,    // 01011110: BIT 3, (HL)
                        &iBitNReg       // 01011111: BIT 3, A
                    },
                    // y = 4
                    {
                        &iBitNReg,      // 01100000: BIT 4, B
                        &iBitNReg,      // 01100001: BIT 4, C
                        &iBitNReg,      // 01100010: BIT 4, D
                        &iBitNReg,      // 01100011: BIT 4, E
                        &iBitNReg,      // 01100100: BIT 4, H
                        &iBitNReg,      // 01100101: BIT 4, L
                        &iBitNPtrHl,    // 01100110: BIT 4, (HL)
                        &iBitNReg       // 01100111: BIT 4, A
                    },
                    // y = 5
                    {
                        &iBitNReg,      // 01101000: BIT 5, B
                        &iBitNReg,      // 01101001: BIT 5, C
                        &iBitNReg,      // 01101010: BIT 5, D
                        &iBitNReg,      // 01101011: BIT 5, E
                        &iBitNReg,      // 01101100: BIT 5, H
                        &iBitNReg,      // 01101101: BIT 5, L
                        &iBitNPtrHl,    // 01101110: BIT 5, (HL)
                        &iBitNReg       // 01101111: BIT 5, A
                    },
                    // y = 6
                    {
                        &iBitNReg,      // 01110000: BIT 6, B
                        &iBitNReg,      // 01110001: BIT 6, C
                        &iBitNReg,      // 01110010: BIT 6, D
                        &iBitNReg,      // 01110011: BIT 6, E
                        &iBitNReg,      // 01110100: BIT 6, H
                        &iBitNReg,      // 01110101: BIT 6, L
                        &iBitNPtrHl,    // 01110110: BIT 6, (HL)
                        &iBitNReg       // 01110111: BIT 6, A
                    },
                    // y = 7
                    {
                        &iBitNReg,      // 01111000: BIT 7, B
                        &iBitNReg,      // 01111001: BIT 7, C
                        &iBitNReg,      // 01111010: BIT 7, D
                        &iBitNReg,      // 01111011: BIT 7, E
                        &iBitNReg,      // 01111100: BIT 7, H
                        &iBitNReg,      // 01111101: BIT 7, L
                        &iBitNPtrHl,    // 01111110: BIT 7, (HL)
                        &iBitNReg       // 01111111: BIT 7, A
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iResNReg,      // 10000000: RES 0, B
                        &iResNReg,      // 10000001: RES 0, C
                        &iResNReg,      // 10000010: RES 0, D
                        &iResNReg,      // 10000011: RES 0, E
                        &iResNReg,      // 10000100: RES 0, H
                        &iResNReg,      // 10000101: RES 0, L
                        &iResNPtrHl,    // 10000110: RES 0, (HL)
                        &iResNReg       // 10000111: RES 0, A
                    },
                    // y = 1
                    {
                        &iResNReg,      // 10001000: RES 1, B
                        &iResNReg,      // 10001001: RES 1, C
                        &iResNReg,      // 10001010: RES 1, D
                        &iResNReg,      // 10001011: RES 1, E
                        &iResNReg,      // 10001100: RES 1, H
                        &iResNReg,      // 10001101: RES 1, L
                        &iResNPtrHl,    // 10001110: RES 1, (HL)
                        &iResNReg       // 10001111: RES 1, A
                    },
                    // y = 2
                    {
                        &iResNReg,      // 10010000: RES 2, B
                        &iResNReg,      // 10010001: RES 2, C
                        &iResNReg,      // 10010010: RES 2, D
                        &iResNReg,      // 10010011: RES 2, E
                        &iResNReg,      // 10010100: RES 2, H
                        &iResNReg,      // 10010101: RES 2, L
                        &iResNPtrHl,    // 10010110: RES 2, (HL)
                        &iResNReg       // 10010111: RES 2, A
                    },
                    // y = 3
                    {
                        &iResNReg,      // 10011000: RES 3, B
                        &iResNReg,      // 10011001: RES 3, C
                        &iResNReg,      // 10011010: RES 3, D
                        &iResNReg,      // 10011011: RES 3, E
                        &iResNReg,      // 10011100: RES 3, H
                        &iResNReg,      // 10011101: RES 3, L
                        &iResNPtrHl,    // 10011110: RES 3, (HL)
                        &iResNReg       // 10011111: RES 3, A
                    },
                    // y = 4
                    {
                        &iResNReg,      // 10100000: RES 4, B
                        &iResNReg,      // 10100001: RES 4, C
                        &iResNReg,      // 10100010: RES 4, D
                        &iResNReg,      // 10100011: RES 4, E
                        &iResNReg,      // 10100100: RES 4, H
                        &iResNReg,      // 10100101: RES 4, L
                        &iResNPtrHl,    // 10100110: RES 4, (HL)
                        &iResNReg       // 10100111: RES 4, A
                    },
                    // y = 5
                    {
                        &iResNReg,      // 10101000: RES 5, B
                        &iResNReg,      // 10101001: RES 5, C
                        &iResNReg,      // 10101010: RES 5, D
                        &iResNReg,      // 10101011: RES 5, E
                        &iResNReg,      // 10101100: RES 5, H
                        &iResNReg,      // 10101101: RES 5, L
                        &iResNPtrHl,    // 10101110: RES 5, (HL)
                        &iResNReg       // 10101111: RES 5, A
                    },
                    // y = 6
                    {
                        &iResNReg,      // 10110000: RES 6, B
                        &iResNReg,      // 10110001: RES 6, C
                        &iResNReg,      // 10110010: RES 6, D
                        &iResNReg,      // 10110011: RES 6, E
                        &iResNReg,      // 10110100: RES 6, H
                        &iResNReg,      // 10110101: RES 6, L
                        &iResNPtrHl,    // 10110110: RES 6, (HL)
                        &iResNReg       // 10110111: RES 6, A
                    },
                    // y = 7
                    {
                        &iResNReg,      // 10111000: RES 7, B
                        &iResNReg,      // 10111001: RES 7, C
                        &iResNReg,      // 10111010: RES 7, D
                        &iResNReg,      // 10111011: RES 7, E
                        &iResNReg,      // 10111100: RES 7, H
                        &iResNReg,      // 10111101: RES 7, L
                        &iResNPtrHl,    // 10111110: RES 7, (HL)
                        &iResNReg       // 10111111: RES 7, A
                    }
                },
                // x = 3
                {
                    // y = 0
                    {
                        &iSetNReg,      // 11000000: SET 0, B
                        &iSetNReg,      // 11000001: SET 0, C
                        &iSetNReg,      // 11000010: SET 0, D
                        &iSetNReg,      // 11000011: SET 0, E
                        &iSetNReg,      // 11000100: SET 0, H
                        &iSetNReg,      // 11000101: SET 0, L
                        &iSetNPtrHl,    // 11000110: SET 0, (HL)
                        &iSetNReg       // 11000111: SET 0, A
                    },
                    // y = 1
                    {
                        &iSetNReg,      // 11001000: SET 1, B
                        &iSetNReg,      // 11001001: SET 1, C
                        &iSetNReg,      // 11001010: SET 1, D
                        &iSetNReg,      // 11001011: SET 1, E
                        &iSetNReg,      // 11001100: SET 1, H
                        &iSetNReg,      // 11001101: SET 1, L
                        &iSetNPtrHl,    // 11001110: SET 1, (HL)
                        &iSetNReg       // 11001111: SET 1, A
                    },
                    // y = 2
                    {
                        &iSetNReg,      // 11010000: SET 2, B
                        &iSetNReg,      // 11010001: SET 2, C
                        &iSetNReg,      // 11010010: SET 2, D
                        &iSetNReg,      // 11010011: SET 2, E
                        &iSetNReg,      // 11010100: SET 2, H
                        &iSetNReg,      // 11010101: SET 2, L
                        &iSetNPtrHl,    // 11010110: SET 2, (HL)
                        &iSetNReg       // 11010111: SET 2, A
                    },
                    // y = 3
                    {
                        &iSetNReg,      // 11011000: SET 3, B
                        &iSetNReg,      // 11011001: SET 3, C
                        &iSetNReg,      // 11011010: SET 3, D
                        &iSetNReg,      // 11011011: SET 3, E
                        &iSetNReg,      // 11011100: SET 3, H
                        &iSetNReg,      // 11011101: SET 3, L
                        &iSetNPtrHl,    // 11011110: SET 3, (HL)
                        &iSetNReg       // 11011111: SET 3, A
                    },
                    // y = 4
                    {
                        &iSetNReg,      // 11100000: SET 4, B
                        &iSetNReg,      // 11100001: SET 4, C
                        &iSetNReg,      // 11100010: SET 4, D
                        &iSetNReg,      // 11100011: SET 4, E
                        &iSetNReg,      // 11100100: SET 4, H
                        &iSetNReg,      // 11100101: SET 4, L
                        &iSetNPtrHl,    // 11100110: SET 4, (HL)
                        &iSetNReg       // 11100111: SET 4, A
                    },
                    // y = 5
                    {
                        &iSetNReg,      // 11101000: SET 5, B
                        &iSetNReg,      // 11101001: SET 5, C
                        &iSetNReg,      // 11101010: SET 5, D
                        &iSetNReg,      // 11101011: SET 5, E
                        &iSetNReg,      // 11101100: SET 5, H
                        &iSetNReg,      // 11101101: SET 5, L
                        &iSetNPtrHl,    // 11101110: SET 5, (HL)
                        &iSetNReg       // 11101111: SET 5, A
                    },
                    // y = 6
                    {
                        &iSetNReg,      // 11110000: SET 6, B
                        &iSetNReg,      // 11110001: SET 6, C
                        &iSetNReg,      // 11110010: SET 6, D
                        &iSetNReg,      // 11110011: SET 6, E
                        &iSetNReg,      // 11110100: SET 6, H
                        &iSetNReg,      // 11110101: SET 6, L
                        &iSetNPtrHl,    // 11110110: SET 6, (HL)
                        &iSetNReg       // 11110111: SET 6, A
                    },
                    // y = 7
                    {
                        &iSetNReg,      // 11111000: SET 7, B
                        &iSetNReg,      // 11111001: SET 7, C
                        &iSetNReg,      // 11111010: SET 7, D
                        &iSetNReg,      // 11111011: SET 7, E
                        &iSetNReg,      // 11111100: SET 7, H
                        &iSetNReg,      // 11111101: SET 7, L
                        &iSetNPtrHl,    // 11111110: SET 7, (HL)
                        &iSetNReg       // 11111111: SET 7, A
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
