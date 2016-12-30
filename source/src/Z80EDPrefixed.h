#pragma once

/** Z80Unprefixed.h
 *
 * Z80 instruction table for unprefixed opcodes.
 *
 */

#include "Z80Instruction.h"
#include "Z80Inst0000Nop.h"

class Z80Unprefixed
{
    public:
        // Instructions
        Z80Inst0000Nop iNop;

        Z80Instruction* table[4][4][2][8];

        Z80Unprefixed() :
            table {
                // x = 0
                {
                    // p = 0
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 1
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 2
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 3
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                },
                // x = 1
                {
                    // p = 0
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 1
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 2
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 3
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                },
                // x = 2
                {
                    // p = 0
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 1
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 2
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 3
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                },
                // x = 3
                {
                    // p = 0
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 1
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 2
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
                    // p = 3
                    {
                        // q = 0
                        {
                            &iNop, // 00000000: NOP
                            &iNop, // 00000001: LD rp[p], nn
                            &iNop, // 00000010: LD (BC), A
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop,
                            &iNop
                        },
                        // q = 1
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
            }
        {}

};

// vim: et:sw=4:ts=4
