#pragma once

/** Z80AdcReg.h
 *
 * Instruction: ADC A, r
 *
 */

bool z80AdcReg()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;

            acc.w = af.l & FLAG_C;
            acc.w += af.h + *reg8[z];

            af.l = acc.l & (FLAG_S | FLAG_5 | FLAG_3);
            af.l |= (acc.l ^ *reg8[z] ^ af.h) & FLAG_H;
            af.l |= (((acc.l ^ *reg8[z] ^ af.h) >> 5) 
                    ^ (acc.h << 2)) & FLAG_PV;
            af.l |= acc.h & FLAG_C;                   // S.5H3V0C
            af.l |= (acc.l) ? 0x00 : FLAG_Z;          // SZ5H3V0C
            af.h = acc.l;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
