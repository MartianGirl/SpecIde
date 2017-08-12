#pragma once

/** Z80BitNPtrHl.h
 *
 * Instruction: BIT n, (HL)
 *
 * Encoding: 11 001 011  01 bbb 110
 * M Cycles: 2 (OCF, OCF, MRB(4))
 * T States: 12
 *
 */

bool z80BitNPtrHl()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 1;
            memAddrMode = 0x00000002;
            return true;

        case 1:
            acc.l = iReg.h;
            acc.l &= (1 << y);

            af.l &= FLAG_C;
            af.l |= FLAG_H;
            af.l |= acc.l & FLAG_S;
            af.l |= tmp.h & (FLAG_5 | FLAG_3);
            af.l |= (acc.l) ? 0x00 : FLAG_Z | FLAG_PV;
            return false;

        case 2:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
