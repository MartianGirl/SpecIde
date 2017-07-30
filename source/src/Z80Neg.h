#pragma once

/** Z80Neg.h
 *
 * Instruction: NEG
 *
 */

bool z80Neg()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;

            // Calculate half-carry. This is done by doing a 4-bit
            // subtraction. Half-carry will be in bit 4.
            acc.w = -(af.h & 0x0F);
            af.l = 
                (acc.w & (FLAG_H | FLAG_3)) | FLAG_N;    // ...H3.1.  

            // Calculate carry in the bit 7. Overflow flag is
            // (carry in bit 7) XOR (carry in bit 8).
            acc.w = -(af.h & 0x7F);
            af.l |= (acc.w >> 5) & FLAG_PV;

            // Calculate the result.
            acc.w = -af.h;
            af.l |= acc.l & (FLAG_S | FLAG_5);        // S.5H3.1.
            af.l |= acc.h & FLAG_C;                   // S.5H3.1C
            af.l ^= (acc.w >> 6) & FLAG_PV;           // S.5H3V1C
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V1C
            af.h = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
