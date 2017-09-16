#pragma once

/** Z80RetI.h
 *
 * Instruction: RETI
 * Instruction: RETN
 *
 */

bool z80RetI()
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
            iff = ((iff & IFF2) >> 2) | (iff & IFF2);
            pc.w = tmp.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
