#pragma once

/** Z80AddIxRegX.h
 *
 * Instruction: ADD IX, rr
 *
 */

bool z80AddIxRegX()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            cpuProcCycles = 2;
            return true;

        case 1:
            // Save HL and operand.
            wz.w = ix.w;
            acc.w = *regpx[p];
            return false;

        case 2:
            // First, do the low byte addition. Carry is in lowest
            // bit of H. Add carry here.
            ix.w = acc.l + wz.l;
            acc.w = acc.h;
            af.l &= (FLAG_S | FLAG_Z | FLAG_PV);
            af.l |= ix.h & FLAG_C;

            // Perform the addition in H, including low byte carry.
            ix.h = acc.l + wz.h + (af.l & FLAG_C);
            return false;

        case 3:
            // Half carry
            af.l |= (wz.h ^ acc.l ^ ix.h) & FLAG_H;
            return false;

        case 4:
            // Carry
            acc.w += wz.h + (af.l & FLAG_C);
            af.l &= ~FLAG_C;
            af.l |= (acc.h & FLAG_C);
            return true;

        case 5:
            // 5 and 3 are affected by the high byte.
            af.l |= ix.h & (FLAG_5 | FLAG_3);
            return false;

        case 6:
            return false;

        case 7:
            ++wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
