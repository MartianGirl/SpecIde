#pragma once

/** Z80Ret.h
 *
 * Instruction: RET
 *
 */

bool z80Ret()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 0;
            memAddrMode = 0x000000BB;
            return true;

        case 1:
            return true;

        case 2:
            pc.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
