#pragma once

/** Z80InAPtrByte.h
 *
 * Instruction: IN A, (n)
 *
 * Encoding: 11 011 011
 * M Cycles: 3 (OCF, MRB, IORD)
 * T States: 11
 *
 */

bool z80InAPtrByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            ioRdCycles = 1;
            memAddrMode = 0x000000D1;
            return true;

        case 1:
            return true;

        case 2:
            af.h = iReg.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
