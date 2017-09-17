#pragma once

/** Z80ExPtrSpIy.h
 *
 * Instruction: EX (SP), IY
 *
 * Encoding: 11 111 101  11 100 011
 * M Cycles: 6 (OCF, OCF, SRB(3), SRB(4), SWB(3), SWB(5))
 * T States: 23
 *
 */

bool z80ExPtrSpIy()
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
            oReg.l = iy.h;
            oReg.h = iy.l;
            return false;

        case 3:
            return true;

        case 4:
            return true;

        case 5:
            return false;

        case 6:
            iy.w = wz.w = iReg.w;
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
