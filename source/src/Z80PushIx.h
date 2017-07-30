#pragma once

/** Z80PushIx.h
 *
 * Instruction: PUSH IX
 *
 * Encoding: 11 011 101  11 100 101
 * M Cycles: 4 (DD, OCF, MWL, MWH)
 * T States: 15
 *
 */

bool z80PushIx()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            return false;

        case 1:
            oReg.l = ix.h;
            oReg.h = ix.l;
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
