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
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                },
                // x = 2
                {
                    // y = 0
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                },
                // x = 3
                {
                    // y = 0
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 1
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 2
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 3
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 4
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 5
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 6
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    },
                    // y = 7
                    {
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop,
                        &iNop
                    }
                }
            }
        {}

};

// vim: et:sw=4:ts=4
