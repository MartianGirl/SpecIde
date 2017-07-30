#pragma once

/** Z80CpRegY.h
 *
 * Instruction: CP ry
 *
 */

bool z80CpRegY()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            tmp.l = *regy8[z];
            acc.w = af.h - tmp.l;

            // Flags 5 & 3 are copied from the operand.
            af.l = tmp.l & (FLAG_5 | FLAG_3);
            af.l |= acc.l & FLAG_S;
            af.l |= acc.h & FLAG_C;
            af.l |= FLAG_N;
            af.l |= (acc.l ^ af.h ^ tmp.l) & FLAG_H;
            af.l |= (((acc.l ^ tmp.l ^ af.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= acc.l ? 0x00 : FLAG_Z;            // SZ5H3VNC
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
