#pragma once

/** Z80CpByte.h
 *
 * Instruction: CP n
 *
 */

bool z80CpByte()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000001;
            return true;

        case 1:
            acc.w = af.h - iReg.h;

            // Flags 5 & 3 are copied from the operand.
            af.l = iReg.h & (FLAG_5 | FLAG_3);
            af.l |= acc.l & FLAG_S;
            af.l |= acc.h & FLAG_C;
            af.l |= FLAG_N;
            af.l |= (acc.l ^ af.h ^ iReg.h) & FLAG_H;
            af.l |= (((acc.l ^ iReg.h ^ af.h) >> 5) 
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
