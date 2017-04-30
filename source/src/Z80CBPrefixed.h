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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
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
                        &iNop,
                        &iSrlReg        // 00111111: SRL A
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
