#pragma once

/** Z80PushIy.h
 *
 * Instruction: PUSH IY
 *
 * Encoding: 11 111 101  11 100 101
 * M Cycles: 4 (FD, OCF, MWL, MWH)
 * T States: 15
 *
 */

bool z80PushIy()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            return false;

        case 1:
            oReg.l = iy.h;
            oReg.h = iy.l;
            return true;

        case 2:
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
