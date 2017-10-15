#pragma once

/** Z80BitNReg.h
 *
 * Instruction: BIT n, r
 *
 * Encoding: 11 001 011  01 bbb rrr
 * M Cycles: 2 (OCF, OCF)
 * T States: 8
 *
 * Reg rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  A  111
 *
 */

bool z80BitNReg()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.l = *reg8[z];

            af.l &= FLAG_C;
            af.l |= FLAG_H;
            af.l |= acc.l & (FLAG_5 | FLAG_3);

            acc.l &= (1 << y);
            af.l |= acc.l & FLAG_S;
            af.l |= (acc.l != 0x00) ? 0x00 : (FLAG_Z | FLAG_PV);

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
