#pragma once

/** Z80ExPtrSpIx.h
 *
 * Instruction: EX (SP), IX
 *
 * Encoding: 11 011 101  11 100 011
 * M Cycles: 6 (OCF, OCF, SRB(3), SRB(4), SWB(3), SWB(5))
 * T States: 23
 *
 */

bool z80ExPtrSpIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 2;
            memAddrMode = 0x0000AABB;
            return true;

        case 1:
            return true;

        case 2:
            oReg.l = ix.h;
            oReg.h = ix.l;
            return false;

        case 3:
            return true;

        case 4:
            return true;

        case 5:
            return false;

        case 6:
            ix.w = tmp.w = iReg.w;
            return false;

        case 7:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
