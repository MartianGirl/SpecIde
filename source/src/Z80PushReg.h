#pragma once

/** Z80PushReg.h
 *
 * Instruction: PUSH rr
 *
 * Encoding: 11 rr0 101
 * M Cycles: 3 (OCF, MWL, MWH)
 * T States: 11
 *
 *  Reg rr
 * --------
 *  BC  00
 *  DE  01
 *  HL  10
 *  AF  11
 *
 */

bool z80PushReg()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            acc.w = *(regp2[p]);
            return false;

        case 1:
            oReg.l = acc.h;
            oReg.h = acc.l;
            return true;

        case 2:
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
