#pragma once

/** Z80LdAI.h
 *
 * Instruction: LD A, I
 *
 * Encoding: 11 101 101  01 010 111
 * M Cycles: 2 (ED, OCF)
 * T States: 9
 *
 * Flags: SZ503P0-*
 *
 * - PF <- IFF2
 *
 */

bool z80LdAI()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x00000000;
            return false;

        case 1:
            af.h = ir.h;
            af.l &= FLAG_C;                                 // 0000000C
            af.l |= (ir.h & (FLAG_S | FLAG_5 | FLAG_3)); // S050300C
            af.l |= (ir.h == 0x00) ? FLAG_Z : 0x00;      // SZ50300C
            af.l |= (iff & IFF2);                        // SZ503P0C
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
