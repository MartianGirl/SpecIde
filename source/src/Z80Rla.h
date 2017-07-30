#pragma once

/** Z80Rla.h
 *
 * Instruction: RLA
 *
 * Encoding: 00 010 111
 * M Cycles: 1 (OCF)
 * T States: 4
 *
 * Flags: **5*3*0C
 * - MSB is copied into CF. CF is copied into LSB.
 *
 */

bool z80Rla()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = af.h << 1;
            af.h = acc.l | (af.l & FLAG_C);
            af.l &= FLAG_S | FLAG_Z | FLAG_PV;
            af.l |= af.h & (FLAG_5 | FLAG_3);
            af.l |= acc.h & FLAG_C;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
