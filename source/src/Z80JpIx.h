#pragma once

/** Z80JpIx.h
 *
 * JP (IX) Instruction.
 *
 */

bool z80JpIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;

            pc.w = ix.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
