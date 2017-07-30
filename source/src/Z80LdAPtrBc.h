#pragma once

/** Z80LdAPtrBc.h
 *
 * Instruction: LD A, (BC)
 *
 * Encoding: 00 001 010
 * M Cycles: 2 (OCF, MRB)
 * T States: 7
 *
 */

bool z80LdAPtrBc()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000003;
            return true;

        case 1:
            af.h = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
