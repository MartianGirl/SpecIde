#pragma once

/** Z80AddRegY.h
 *
 * Instruction: ADD A, ry
 *
 */

bool z80AddRegY()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = af.h + *regy8[z];

            af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ *regy8[z] ^ af.h) & FLAG_H;
            af.l |= (((acc.l ^ *regy8[z] ^ af.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= acc.h & FLAG_C;                   // S.5H3V0C
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
            af.h = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
