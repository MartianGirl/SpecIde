#pragma once

/** Z80Rst.h
 *
 * Instruction: RST 0h
 * Instruction: RST 8h
 * Instruction: RST 10h
 * Instruction: RST 18h
 * Instruction: RST 20h
 * Instruction: RST 28h
 * Instruction: RST 30h
 * Instruction: RST 38h
 *
 */

bool z80Rst()
{
    switch (executionStep)
    {
        case 0:
            memWrCycles = 2;
            memAddrMode = 0x000000AA;
            return false;

        case 1:
            oReg.l = pc.h;
            oReg.h = pc.l;
            return true;

        case 2:
            return true;

        case 3:
            pc.w = tmp.w = (y << 3);
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
