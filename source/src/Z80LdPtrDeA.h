#pragma once

/** Z80LdPtrDeA.h
 *
 * Instruction: LD (DE), A
 *
 * Encoding: 00 010 010
 * M Cycles: 2 (OCF, MWB)
 * T States: 7
 *
 */

bool z80LdPtrDeA()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 1;
            memAddrMode = 0x00000004;

            wz.w = de.w + 1;
            oReg.l = af.h;
            return true;

        case 1:
            wz.h = af.h;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
