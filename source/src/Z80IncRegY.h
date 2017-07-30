#pragma once

/** Z80IncRegY.h
 *
 * Instruction: INC ry
 *
 */

bool z80IncRegY()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            tmp.l = *regy8[y];
            acc.w = tmp.l + 1;

            // Preserve carry flag.
            af.l &= FLAG_C;
            af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ tmp.l) & FLAG_H;
            af.l |= (((acc.l ^ tmp.l) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
            *regy8[y] = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
