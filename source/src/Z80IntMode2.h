#pragma once

/** Z80IntMode2.h
 *
 * Interrupt mode 2 process (modelled as an instruction!)
 *
 */

bool z80IntMode2()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 2;
            memAddrMode = 0x0000AA9C;
            return false;

        case 1:
            wz.w = (ir.h << 8) | d;
            return true;

        case 2:
            return true;

        case 3:
            wz.w = iReg.w;
            acc.w = pc.w;
            oReg.l = acc.h;
            oReg.h = acc.l;
            return true;

        case 4:
            return true;

        case 5:
            pc.w = wz.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
