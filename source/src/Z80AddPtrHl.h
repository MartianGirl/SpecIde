#pragma once

/** Z80AddPtrHl.h
 *
 * Instruction: ADD A, (HL)
 *
 */

bool z80AddPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            af.l = addFlags[0][af.h][iReg.h];
            af.h += iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
