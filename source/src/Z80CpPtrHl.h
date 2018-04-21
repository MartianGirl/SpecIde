#pragma once

/** Z80CpPtrHl.h
 *
 * Instruction: CP (HL)
 *
 */

bool z80CpPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            af.l = cpFlags[af.h][iReg.h];
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
