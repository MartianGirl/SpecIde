#pragma once

/** Z80RetCc.h
 *
 * Instruction: RET NZ
 * Instruction: RET Z
 * Instruction: RET NC
 * Instruction: RET C
 * Instruction: RET PO
 * Instruction: RET PE
 * Instruction: RET P
 * Instruction: RET M
 *
 */

bool z80RetCc()
{
    switch (executionStep)
    {
        case 0:
            memAddrMode = 0x000000BB;
            return false;

        case 1:
            switch (y)
            {
                case 0: memRdCycles = ((af.l & FLAG_Z) == 0x00) ? 2 : 0; break;
                case 1: memRdCycles = ((af.l & FLAG_Z) == FLAG_Z) ? 2 : 0; break;
                case 2: memRdCycles = ((af.l & FLAG_C) == 0x00) ? 2 : 0; break;
                case 3: memRdCycles = ((af.l & FLAG_C) == FLAG_C) ? 2 : 0; break;
                case 4: memRdCycles = ((af.l & FLAG_PV) == 0x00) ? 2 : 0; break;
                case 5: memRdCycles = ((af.l & FLAG_PV) == FLAG_PV) ? 2 : 0; break;
                case 6: memRdCycles = ((af.l & FLAG_S) == 0x00) ? 2 : 0; break;
                case 7: memRdCycles = ((af.l & FLAG_S) == FLAG_S) ? 2 : 0; break;
                default: assert(false); break;
            }
            return true;

        case 2:
            return true;

        case 3:
            pc.w = iReg.w;
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
