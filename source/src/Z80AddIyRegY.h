#pragma once

/** Z80AddIyRegY.h
 *
 * Instruction: ADD IY, rr
 *
 */

bool z80AddIyRegY()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 0;
            memAddrMode = 0x00000000;
            return false;

        case 1:
            // Save HL and operand.
            wz.w = iy.w;
            acc.w = *regpy[p];
            return false;

        case 2:
            // First, do the low byte addition. Carry is in lowest
            // bit of H. Add carry here.
            iy.w = acc.l + wz.l;
            acc.w = acc.h;
            af.l &= (FLAG_S | FLAG_Z | FLAG_PV);
            af.l |= iy.h & FLAG_C;

            // Perform the addition in H, including low byte carry.
            iy.h = acc.l + wz.h + (af.l & FLAG_C);
            return false;

        case 3:
            // Half carry
            af.l |= (wz.h ^ acc.l ^ iy.h) & FLAG_H;
            return false;

        case 4:
            // Carry
            acc.w += wz.h + (af.l & FLAG_C);
            af.l &= ~FLAG_C;
            af.l |= (acc.h & FLAG_C);
            return false;

        case 5:
            // 5 and 3 are affected by the high byte.
            af.l |= iy.h & (FLAG_5 | FLAG_3);
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
