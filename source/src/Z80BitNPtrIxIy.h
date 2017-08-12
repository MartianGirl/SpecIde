#pragma once

/** Z80BitNPtrIxIy.h
 *
 * Instruction: BIT n, (IX + d)
 * Instruction: BIT n, (IY + d)
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  01 bbb rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  01 bbb rrr
 * M Cycles: 5 (OCF, OCF, MRB, MRB(5), MRB(4))
 * T States: 20
 *
 */

bool z80BitNPtrIxIy()
{
    switch (executionStep)
    {
        case 5:
            acc.l = iReg.h;
            acc.l &= (1 << y);
            return false;

        case 6:
            af.l &= FLAG_C;
            af.l |= FLAG_H;
            af.l |= acc.l & FLAG_S;
            af.l |= tmp.h & (FLAG_5 | FLAG_3);
            af.l |= (acc.l) ? 0x00 : FLAG_Z | FLAG_PV;

            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
