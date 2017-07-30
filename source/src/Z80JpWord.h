#pragma once

/** Z80JpWord.h
 *
 * JP nn Instruction.
 *
 */

bool z80JpWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 0;
            memAddrMode = 0x00000011;
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
