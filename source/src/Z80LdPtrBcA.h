#pragma once

/** Z80LdPtrBcA.h
 *
 * Instruction: LD (BC), A
 *
 * Encoding: 00 000 010
 * M Cycles: 2 (OCF, MWB)
 * T States: 7
 *
 */

bool z80LdPtrBcA()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 1;
            memAddrMode = 0x00000003;

            oReg.l = af.h;
            return true;

        case 1:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
