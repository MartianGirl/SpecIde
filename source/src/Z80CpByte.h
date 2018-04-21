#pragma once

/** Z80CpByte.h
 *
 * Instruction: CP n
 *
 */

bool z80CpByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
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
