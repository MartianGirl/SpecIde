#pragma once

/** Z80Rlca.h
 *
 * Instruction: RLCA
 *
 * Encoding: 00 000 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - MSB is copied into LSB and into CF.
 *
 */

bool z80Rlca()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = af.h << 1;
            af.h = acc.l | acc.h;
            af.l &= FLAG_S | FLAG_Z | FLAG_PV;
            af.l |= af.h & (FLAG_5 | FLAG_3 | FLAG_C);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
