#pragma once

/** Z80Nmi.h
 *
 * NMI process (modelled as an instruction!)
 *
 */

bool z80Nmi()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 0;
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            acc.w = pc.w;
            return false;

        case 1:
            oReg.l = acc.h;
            oReg.h = acc.l;
            return true;

        case 2:
            return true;

        case 3:
            pc.w = 0x0066;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
