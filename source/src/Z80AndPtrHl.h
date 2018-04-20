#pragma once

/** Z80AndPtrHl.h
 *
 * Instruction: AND (HL)
 *
 */

bool z80AndPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            // Calculate the result.
            af.l = andFlags[af.h][iReg.h];
            af.h &= iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
