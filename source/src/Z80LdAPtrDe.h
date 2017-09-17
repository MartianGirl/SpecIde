#pragma once

/** Z80LdAPtrDe.h
 *
 * Instruction: LD A, (DE)
 *
 * Encoding: 00 011 010
 * M Cycles: 2 (OCF, MRB)
 * T States: 7
 *
 */

bool z80LdAPtrDe()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000004;

            wz.w = de.w + 1;
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
