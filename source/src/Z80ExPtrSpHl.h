#pragma once

/** Z80ExPtrSpHl.h
 *
 * Instruction: EX (SP), HL
 *
 * Encoding: 11 100 011
 * M Cycles: 5 (OCF, SRB(3), SRB(4), SWB(3), SWB(5))
 * T States: 19
 *
 */

bool z80ExPtrSpHl()
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
            oReg.l = hl.h;
            oReg.h = hl.l;
            return false;

        case 3:
            return true;

        case 4:
            return true;

        case 5:
            return false;

        case 6:
            hl.w = wz.w = iReg.w;
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
