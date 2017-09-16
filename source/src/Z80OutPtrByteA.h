#pragma once

/** Z80OutPtrByteA.h
 *
 * Instruction: OUT (n), A
 *
 * Encoding: 11 010 011
 * M Cycles: 3 (OCF, MRB, IOWR)
 * T States: 11
 *
 */

bool z80OutPtrByteA()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            ioWrCycles = 1;
            memAddrMode = 0x000000D1;
            return true;

        case 1:
            tmp.w = (af.h << 8) | iReg.h;
            oReg.l = af.h;
            return true;

        case 2:
            tmp.h = af.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
