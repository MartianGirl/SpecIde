#pragma once

/** Z80PrefixDDCB.h
 *
 * Prefix DDCB.
 *
 */

bool z80PrefixDDCB()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memAddrMode = 0x00006611;
            return true;

        case 1: // This is the displacement.
            wz.l = iReg.h;
            return true;

        case 2: // This is actually the opcode. Now we *decode* it,
            // as if we were starting a new instruction.
            // Also, during this cycle we calculate the operand
            // address.
            wz.h = ((wz.l & 0x80) == 0x80) ? 0xFF : 0x00;
            return false;

        case 3:
            wz.w += ix.w;
            return false;

        case 4:
            opcode = iReg.h;
            x = (iReg.h & 0xC0) >> 6; // xx......
            y = (iReg.h & 0x38) >> 3; // ..yyy...
            z = (iReg.h & 0x07);      // .....zzz
            p = y >> 1;               // ..pp....
            q = y & 0x01;             // ....q...

            memRdCycles = 1;
            memWrCycles = (x == 1) ? 0 : 1;
            ioRdCycles = 0;
            ioWrCycles = 0;
            cpuProcCycles = 0;

            prefix = PREFIX_DD | PREFIX_CB;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}
// vim: et:sw=4:ts=4
