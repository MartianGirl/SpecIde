#pragma once

/** Z80DecPtrIy.h
 *
 * Instruction: DEC (IY+d)
 *
 */

bool z80DecPtrIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memWrCycles = 0;
            memAddrMode = 0x00000771;
            return true;

        case 1:
            wz.l = iReg.h;
            return false;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += iy.w;
            return false;

        case 4:
            return false;

        case 5:
            memRdCycles = 1;
            return true;

        case 6:
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

        case 7:
            oReg.l = acc.l;
            memWrCycles = 1;
            return true;

        case 8:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
