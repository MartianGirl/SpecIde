#pragma once

/** Z80SubByte.h
 *
 * Instruction: SUB n
 *
 */

bool z80SubByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            af.l = subFlags[0][af.h][iReg.h];
            af.h -= iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
