#pragma once

/** Z80IncPtrHl.h
 *
 * Instruction: INC (HL)
 *
 */

bool z80IncPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000022;
            return true;

        case 1:
            acc.w = iReg.h + 1;

            // Preserve carry flag.
            af.l &= FLAG_C;
            af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ iReg.h) & FLAG_H;
            af.l |= (((acc.l ^ iReg.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
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
