#pragma once

/** Z80JpCcWord.h
 *
 * JP NZ, nn Instruction.
 * JP Z, nn Instruction.
 * JP NC, nn Instruction.
 * JP C, nn Instruction.
 * JP PO, nn Instruction.
 * JP PE, nn Instruction.
 * JP P, nn Instruction.
 * JP M, nn Instruction.
 *
 */

bool z80JpCcWord()
{
    switch (executionStep)
    {
        case 0:
            memRdCycles = 2;
            memWrCycles = 0;
            memAddrMode = 0x00000011;
            return true;

        case 1:
            return true;

        case 2:
            switch (y)
            {
                case 0:
                    if ((af.l & FLAG_Z) == 0x00)
                        pc.w = iReg.w;
                    break;
                case 1:
                    if ((af.l & FLAG_Z) == FLAG_Z)
                        pc.w = iReg.w;
                    break;
                case 2:
                    if ((af.l & FLAG_C) == 0x00)
                        pc.w = iReg.w;
                    break;
                case 3:
                    if ((af.l & FLAG_C) == FLAG_C)
                        pc.w = iReg.w;
                    break;
                case 4:
                    if ((af.l & FLAG_PV) == 0x00)
                        pc.w = iReg.w;
                    break;
                case 5: 
                    if ((af.l & FLAG_PV) == FLAG_PV)
                        pc.w = iReg.w;
                    break;
                case 6:
                    if ((af.l & FLAG_S) == 0x00)
                        pc.w = iReg.w;
                    break;
                case 7:
                    if ((af.l & FLAG_S) == FLAG_S)
                        pc.w = iReg.w;
                    break;
                default: assert(false);
            }
            prefix = PREFIX_NO;
            return true;

        default:    // Should not happen
            assert(false);
            return true;
    }
}

// vim: et:sw=4:ts=4
