#pragma once

/** Z80Cpl.h
 *
 * Instruction: CPL
 *
 */

bool z80Cpl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;

            af.h = ~af.h;
            af.l &= (FLAG_S | FLAG_Z | FLAG_PV | FLAG_C);
            af.l |= af.h & (FLAG_5 | FLAG_3);
            af.l |= FLAG_H | FLAG_N;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
