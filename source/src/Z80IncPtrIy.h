#pragma once

/** Z80IncPtrIy.h
 *
 * Instruction: INC (IY+d)
 *
 */

bool z80IncPtrIy()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            cpuProcCycles = 1;
            memAddrMode = 0x00000771;
            return true;

        case 1:
            wz.l = iReg.h;
            return true;

        case 2:
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += iy.w;
            return false;

        case 4:
        case 5:
            return false;

        case 6:
            memRdCycles = 1;
            return true;

        case 7:
            acc.w = iReg.h + 1;

            // Preserve carry flag.
            af.l &= FLAG_C;
            af.l |= acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ iReg.h) & FLAG_H;
            af.l |= (((acc.l ^ iReg.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0.
            return false;

        case 8:
            oReg.l = acc.l;
            memWrCycles = 1;
            return true;

        case 9:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
