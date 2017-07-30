#pragma once

/** Z80SetNPtrIxIy.h
 *
 * Instruction: SET n, (IX + d)
 * Instruction: SET n, (IX + d), r
 * Instruction: SET n, (IY + d)
 * Instruction: SET n, (IY + d), r
 *
 * Encoding: 11 011 101  11 001 011  dd ddd ddd  11 bbb rrr
 * Encoding: 11 111 101  11 001 011  dd ddd ddd  11 bbb rrr
 * M Cycles: 6 (OCF, OCF, MRB(5), MRB, MRB(4), MWB)
 * T States: 23
 *
 *  r  rrr
 * --- ---
 *  B  000
 *  C  001
 *  D  010
 *  E  011
 *  H  100
 *  L  101
 *  -  110
 *  A  111
 *
 */

bool z80SetNPtrIxIy()
{
    switch (executionStep)
    {
        case 5:
            acc.l = iReg.h;
            acc.l |= (1 << y);
            if (z != 6)
                *reg8[z] = acc.l;
            return false;

        case 6:
            oReg.l = acc.l;
            return true;

        case 7:
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
