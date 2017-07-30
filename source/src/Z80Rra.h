#pragma once

/** Z80Rra.h
 *
 * Instruction: RRA
 *
 * Encoding: 00 011 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - LSB is copied into CF. CF is copied into MSB.
 *
 */

bool z80Rra()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.l = af.h;
            acc.h = af.l & FLAG_C;
            af.l &= FLAG_S | FLAG_Z | FLAG_PV;
            af.l |= af.h & FLAG_C;
            acc.w >>= 1;
            af.h = acc.l;
            af.l |= af.h & (FLAG_5 | FLAG_3);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
