#pragma once

/** Z80DecPtrHl.h
 *
 * Instruction: DEC (HL)
 *
 */

bool z80DecPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 0;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            // Preserve carry bit.
            af.l &= FLAG_C;

            // Calculate half-carry. This is done by doing a 4-bit
            // subtraction. Half-carry will be in bit 4.
            acc.w = (iReg.h & 0x0F) - 1;
            af.l |= 
                (acc.w & (FLAG_H | FLAG_3)) | FLAG_N;    // ...H3.1.  

            // Calculate carry in the bit 7. Overflow flag is
            // (carry in bit 7) XOR (carry in bit 8).
            acc.w = (iReg.h & 0x7F) - 1;
            af.l |= (acc.w >> 5) & FLAG_PV;

            // Calculate the result.
            acc.w = iReg.h - 1;
            af.l |= acc.l & (FLAG_S | FLAG_5);        // S.5H3.1.
            af.l ^= (acc.w >> 6) & FLAG_PV;           // S.5H3V1.
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V1.
            return false;

        case 2:
            oReg.l = acc.l;
            memWrCycles = 1;
            return true;

        case 3:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
