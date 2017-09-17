#pragma once

/** Z80IncPtrIx.h
 *
 * Instruction: INC (IX+d)
 *
 */

bool z80IncPtrIx()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00000661;
            return true;

        case 1:
            wz.l = iReg.h;
            return false;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += ix.w;
            return false;

        case 4:
            return false;

        case 5:
            memRdCycles = 1;
            return true;

        case 6:
            acc.w = iReg.h + 1;

            // Preserve carry flag.
            af.l &= FLAG_C;
            af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ iReg.h) & FLAG_H;
            af.l |= (((acc.l ^ iReg.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
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
