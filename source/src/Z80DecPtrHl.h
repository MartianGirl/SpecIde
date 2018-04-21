#pragma once

/** Z80DecPtrHl.h
 *
 * Instruction: DEC (HL)
 *
 */

bool z80DecPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            // Preserve carry bit.
            af.l &= FLAG_C;
            af.l |= decFlags[iReg.h];
            return false;

        case 2:
            oReg.l = iReg.h - 1;
            memWrCycles = 1;
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
