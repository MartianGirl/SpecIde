#pragma once

/** Z80Call.h
 *
 * Instruction: CALL nn
 *
 */

bool z80Call()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 2;
            memAddrMode = 0x0000AA11;
            return true;

        case 1:
            return true;

        case 2:
            return false;

        case 3:
            oReg.l = pc.h;
            oReg.h = pc.l;
            return true;

        case 4:
            return true;

        case 5:
            pc.w = tmp.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
