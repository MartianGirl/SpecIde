/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CRTC.h"

#include <iostream>

using namespace std;

CRTC::CRTC(uint_fast8_t type) :
    type(type),
    index(0),
    regs{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, static_cast<uint_fast8_t>((type == 1) ? 0xFF : 0x00)},
    mask{
        0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x7F, 0x7F,
        0x03, 0x1F, 0x7F, 0x1F, 0x3F, 0xFF, 0x3F, 0xFF,
        0x3F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, static_cast<uint_fast8_t>((type == 1) ? 0xFF : 0x00)},
    dirs{
        AccessType::CRTC_WO,    // R0: Horizontal Total (WO)
        AccessType::CRTC_WO,    // R1: Horizontal Displayed (WO)
        AccessType::CRTC_WO,    // R2: Horizontal Sync Position (WO)
        AccessType::CRTC_WO,    // R3: Horizontal & Vertical Sync Width (WO)
        AccessType::CRTC_WO,    // R4: Vertical Total (WO)
        AccessType::CRTC_WO,    // R5: Vertical Total Adjust (WO)
        AccessType::CRTC_WO,    // R6: Vertical Displayed (WO)
        AccessType::CRTC_WO,    // R7: Vertical Sync Position (WO)
        AccessType::CRTC_WO,    // R8: Interlace & Skew (WO)
        AccessType::CRTC_WO,    // R9: Maximum Raster Address (WO)
        AccessType::CRTC_RW,    // R10: Cursor Start Raster (RW)
        AccessType::CRTC_RW,    // R11: Cursor End Raster (RW)
        (type == 0 || type > 2) ? AccessType::CRTC_RW : AccessType::CRTC_WO,    // R12: Display Address (High)
        (type == 0 || type > 2) ? AccessType::CRTC_RW : AccessType::CRTC_WO,    // R13: Display Address (Low)
        AccessType::CRTC_RW,    // R14: Cursor Address (High byte)
        AccessType::CRTC_RW,    // R15: Cursor Address (Low byte)
        AccessType::CRTC_RO,    // R16: Light Pen Address (High byte)
        AccessType::CRTC_RO,    // R17: Light Pen Address (Low byte)
        AccessType::CRTC_RO,    // All the remaining addresses are unused.
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO,
        AccessType::CRTC_RO} {}

void CRTC::rdStatus(uint_fast8_t &byte) {

    switch (type) {
        case 1:
            // Status reg on CRTC type 1 reflects VDisplayed.
            byte = r6_vDisplayed ? 0x20 : 0x00;
            break;
        case 3: // fall-through
        case 4:
            rdRegister(byte);
            break;
        default:
            break;
    }
}

void CRTC::rdRegister(uint_fast8_t &byte) {

    // CRTC type 3 or 4 decode read address partially.
    uint_fast8_t address = (type > 2) ? ((index & 0x7) | 0x8) : index;

    if (dirs[index] != AccessType::CRTC_WO) {
        switch (address) {
            case 0x1f:
                // R31 on CRTC 0 or 2 returns 0.
                // R31 on CRTC 1 returns the contents of the bus (HiZ).
                // R31 on CRTC 3 or 4 returns R15 because of incomplete decoding.
                if (type != 1) byte = 0x0;
                break;
            default:
                byte = regs[address];
                break;
        }
    } else {
        switch (type) {
            case 0:
                byte = 0x0;
                break;
            case 1:
                if (address == 12 || address == 13) byte = 0x0;
                break;
            case 2:
                byte = 0x0;
                break;
            case 3:
            case 4:
                if (address == 10) {
                    // R10.b5 reflects VSync status.
                    byte = vSync ? 0x20 : 0x00;
                } else if (address == 11) {
                    // R11 reflects VCC.
                    byte = c4_vCounter;
                }
            default:
                break;
        }
    }
}

void CRTC::wrAddress(uint_fast8_t byte) {

    index = byte & 0x1F;
}

void CRTC::wrRegister(uint_fast8_t byte) {

    if (dirs[index] == AccessType::CRTC_WO
            || dirs[index] == AccessType::CRTC_RW) {
        regs[index] = byte & mask[index];

        switch (index) {
            case 0: // Horizontal Total. Actual value = Set value + 1.
                if (type == 0 && regs[0] == 0) {
                    regs[0] = 1;
                }
                r0_hTotal = regs[0];
                break;
            case 1: // Horizontal Displayed.
                r1_hDisplayed = regs[1];
#ifdef DEBUGCRTC
                cout << "Update HDisp(" << static_cast<uint32_t>(r1_hDisplayed)
                    << ") at VCC=" << static_cast<uint32_t>(c4_vCounter)
                    << " VLC=" << static_cast<uint32_t>(c9_rCounter)
                    << " VAD=" << static_cast<uint32_t>(c5_aCounter)
                    << " HCC=" << static_cast<uint32_t>(c0_hCounter) << endl;
#endif
                break;
            case 2: // Horizontal Sync Position.
                r2_hSyncPos = regs[2];
                break;
            case 3: // Horizontal & Vertical Sync Width.
                r3h_vSyncWidth = (regs[3] & 0xF0) >> 4;
                r3l_hSyncWidth = (regs[3] & 0x0F);
                switch (type) {
                    case 1: // Type 1 (UM6845R):
                        // VSW is ignored. VSYNC is fixed to 16 lines.
                        r3h_vSyncWidth = 0x0;
                        break;
                    case 2: // Type 2 (MC6845):
                        // VSW is ignored. VSYNC is fixed to 16 lines.
                        r3h_vSyncWidth = 0x0;
                        break;
                    default:
                        break;
                }
                break;
            case 4: // Vertical total.
                r4_vTotal = regs[4];
#ifdef DEBUGCRTC
                cout << "Update VTotal(" << static_cast<uint32_t>(r4_vTotal)
                    << ") at VCC=" << static_cast<uint32_t>(c4_vCounter)
                    << " VLC=" << static_cast<uint32_t>(c9_rCounter)
                    << " VAD=" << static_cast<uint32_t>(c5_aCounter)
                    << " HCC=" << static_cast<uint32_t>(c0_hCounter) << endl;
#endif
                break;
            case 5: // Vertical adjust.
                r5_vAdjust = regs[5];
#ifdef DEBUGCRTC
                cout << "Update VAdjust(" << static_cast<uint32_t>(r5_vAdjust)
                    << ") at VCC=" << static_cast<uint32_t>(c4_vCounter)
                    << " VLC=" << static_cast<uint32_t>(c9_rCounter)
                    << " VAD=" << static_cast<uint32_t>(c5_aCounter)
                    << " HCC=" << static_cast<uint32_t>(c0_hCounter) << endl;
#endif
                break;
            case 6: // Vertical displayed.
                r6_vDisplayed = regs[6];
#ifdef DEBUGCRTC
                cout << "Update VDisplayed(" << static_cast<uint32_t>(r6_vDisplayed)
                    << ") at VCC=" << static_cast<uint32_t>(c4_vCounter)
                    << " VLC=" << static_cast<uint32_t>(c9_rCounter)
                    << " VAD=" << static_cast<uint32_t>(c5_aCounter)
                    << " HCC=" << static_cast<uint32_t>(c0_hCounter) << endl;
#endif
                break;
            case 7: // Vertical Sync Position.
                r7_vSyncPos = regs[7];
                if (c4_vCounter == r7_vSyncPos) {
                    vSync = true;
                    c3h_vSyncWidth = 0;
                }
                break;
            case 12:    // fall-through
            case 13:
#ifdef DEBUGCRTC
                cout << "Update base address(" << regs[13] + (regs[12] & 0x3F) * 0x100
                    << ") at VCC=" << static_cast<uint32_t>(c4_vCounter)
                    << " VLC=" << static_cast<uint32_t>(c9_rCounter)
                    << " VAD=" << static_cast<uint32_t>(c5_aCounter)
                    << " HCC=" << static_cast<uint32_t>(c0_hCounter) << endl;
#endif
                break;
            default:
                break;
        }
    }

    double base = (1000000.0 / 57.5);
    vSyncSeparation = base / (r0_hTotal ? r0_hTotal : 64);
}

void CRTC::clock() {

    videoOffsetUpdated = false;

    // If Horizontal Character Counter (HCC, C0) matches Horizontal Total (R0),
    // it returns to 0. Otherwise, it is incremented.
    if (c0_hCounter == r0_hTotal) {
        c0_hCounter = 0;
    } else {
        ++c0_hCounter;
    }

    // Some processing occurs on different values of HCC/C0.
    if (c0_hCounter == 0) {
        // Image is displayed again for this line.
        hDisplay = true;
        updateC4();
        updateC5();
        updateC9();

#ifdef DEBUGCRTC
        cout << " C0: " << static_cast<uint32_t>(c0_hCounter)
            << " C4: " << static_cast<uint32_t>(c4_vCounter)
            << " C9: " << static_cast<uint32_t>(c9_rCounter)
            << " C5: " << static_cast<uint32_t>(c5_aCounter)
            << " VP: " << static_cast<uint32_t>(lineAddress)
            << endl;
#endif
        // Check C9 == R9.
        bool lastLineInCharRowBefore = (c9_rCounter == r9_rMaxAddress);
        bool lastLineInCharRowAfter = (c9_rCounter == regs[9]);
        bool lastLineInCharRow = lastLineInCharRowBefore || lastLineInCharRowAfter;
        // Check C4 == R4.
        bool lastCharRow = (c4_vCounter == r4_vTotal);

        if (!processVAdjust) {
            switch (type) {
                case 0: // CRTC Type 0:
                    if (enableRasterCounter) {
                        if (lastCharRow) { // C4 == R4
                            if (lastLineInCharRowBefore) {
                                // C9 and C4 go to 0.
                                // Video offset is updated.
                                // This is the "last line" case, hence if C5 != 0
                                // the vertical adjustment phase is entered.
                                if (r5_vAdjust) {
                                    c9Update = UpdateMode::RESET;
                                    c4Update = UpdateMode::INCREMENT;
                                    processVAdjust = true;
                                } else {
                                    c9Update = UpdateMode::RESET;
                                    c4Update = UpdateMode::RESET;
                                }
                            } else if (lastLineInCharRowAfter) {
                                // C9 goes to 0, C4 is incremented and overflows.
                                c9Update = UpdateMode::RESET;
                                c4Update = UpdateMode::INCREMENT;
                            } else {
                                // C9 is incremented.
                                // Video offset is not updated.
                                c9Update = UpdateMode::INCREMENT;
                            }
                        } else { // General behaviour (C4 != R4)
                            if (lastLineInCharRowAfter) {
                                // This covers C9 = R9 after the update.
                                c9Update = UpdateMode::RESET;
                                c4Update = UpdateMode::INCREMENT;
                            } else {
                                // This covers C9 == R9 before the update. (No update
                                // happened).
                                c9Update = UpdateMode::INCREMENT;
                            }
                        }
                    }
                    enableRasterCounter = false;
                    break;

                case 1: // CRTC Type 1:
                    if (lastCharRow) { // C4 == R4
                        if (lastLineInCharRowAfter) {
                            // C9 goes to 0.
                            // This is the "last line" case, hence if C5 != 0
                            // the vertical adjustment phase is entered.
                            if (r5_vAdjust) {
                                // Set C5=0, increment C4.
                                c9Update = UpdateMode::RESET;
                                c5Update = UpdateMode::RESET;
                                c4Update = UpdateMode::INCREMENT;
                                processVAdjust = true;
                            } else {
                                // C4 goes to 0.
                                // Video offset is updated.
                                c9Update = UpdateMode::RESET;
                                c4Update = UpdateMode::RESET;
                                //updateVideoOffset();
                            }
                        } else {
                            // C9 is incremented.
                            // Video offset is updated if C4 == 0.
                            c9Update = UpdateMode::INCREMENT;
                            //if (!c4_vCounter) updateVideoOffset();
                        }
                    } else { // General behaviour (C4 != R4)
                        if (lastLineInCharRowAfter) {
                            // This covers C9 = R9 after the update.
                            c9Update = UpdateMode::RESET;
                            c4Update = UpdateMode::INCREMENT;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            c9Update = UpdateMode::INCREMENT;
                            //if (!c4_vCounter) updateVideoOffset();
                        }
                    }
                    break;

                case 2: // CRTC Type 2:
                    if (lastCharRow) { // C4 == R4
                        if (lastLineInCharRow) {
                            // C9 and C4 go to 0.
                            // Video offset is updated if C0 <= R1.
                            // This is the "last line" case, hence if C5 != 0
                            // the vertical adjustment phase is entered.
                            c9Update = UpdateMode::RESET;
                            if (r5_vAdjust) {
                                // Set C5=0, increment C4.
                                c9Update = UpdateMode::RESET;
                                c5Update = UpdateMode::RESET;
                                c4Update = UpdateMode::INCREMENT;
                                processVAdjust = true;
                            } else {
                                c9Update = UpdateMode::RESET;
                                c4Update = UpdateMode::RESET;
                                if (c0_hCounter <= r1_hDisplayed) updateVideoOffset();
                            }
                            // TODO: Check for HSYNC cancellation.
                            // It applies to the BEFORE case, when C4 == R4 == 0.
                        } else {
                            // C9 is incremented.
                            // Video offset is not updated.
                            c9Update = UpdateMode::INCREMENT;
                        }
                    } else { // General behaviour (C4 != R4)
                        if (lastLineInCharRowAfter) {
                            // This covers C9 = R9 after the update.
                            c9Update = UpdateMode::RESET;
                            c4Update = UpdateMode::INCREMENT;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            c9Update = UpdateMode::INCREMENT;
                        }
                    }
                    break;

                default: // CRTC 3 & 4.
                    if (lastCharRow) { // C4 == R4
                        if (c9_rCounter >= regs[9]) {
                            if (r5_vAdjust) {
                                c9Update = UpdateMode::RESET;
                                c5Update = UpdateMode::RESET;
                                processVAdjust = true;
                            } else {
                                c9Update = UpdateMode::RESET;
                                c4Update = UpdateMode::RESET;
                                //updateVideoOffset();
                            }
                        } else {
                            // C9 is incremented.
                            // Video offset is not updated.
                            c9Update = UpdateMode::INCREMENT;
                        }
                    } else { // General behaviour (C4 != R4).
                        if (c9_rCounter >= regs[9]) {
                            // This covers C9 = R9 after the update.
                            c9Update = UpdateMode::RESET;
                            c4Update = UpdateMode::INCREMENT;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            c9Update = UpdateMode::INCREMENT;
                        }
                    }
                    break;
            }
        } else { // processVAdjust == true
            // Check C9 == R5.
            bool lastLineInVAdjust = c5_aCounter == r5_vAdjust - 1;

            switch (type) {
                case 0: // CRTC 0.
                    if (c9_rCounter == r5_vAdjust - 1) {
                        // Limit is r5_vAdjust - 1, but we've already incremented.
                        // In this case, the line is corrected.
                        c9Update = UpdateMode::RESET;
                        c4Update = UpdateMode::RESET;
                        processVAdjust = false;
                    } else {
                        c9Update = UpdateMode::INCREMENT;
                    }
                    break;

                case 1: // CRTC 1, 2.
                case 2: // fall-through
                    if (!r5_vAdjust) {
                        c9Update = UpdateMode::RESET;
                        processVAdjust = false;
                    } else if (lastLineInVAdjust) {
                        // Last line of the screen.
                        c9Update = UpdateMode::RESET;
                        c4Update = UpdateMode::RESET;
                        //updateVideoOffset();
                        processVAdjust = false;
                    } else if (lastLineInCharRowAfter) {
                        // This covers C9 = R9 after the update.
                        c9Update = UpdateMode::RESET;
                        c5Update = UpdateMode::INCREMENT;
                        c4Update = UpdateMode::INCREMENT;
                    } else {
                        // This covers C9 == R9 before the update. (No update
                        // happened).
                        c9Update = UpdateMode::INCREMENT;
                        c5Update = UpdateMode::INCREMENT;
                    }
                    break;

                default: // CRTC 3, 4.
                    if (!r5_vAdjust) {
                        c9Update = UpdateMode::RESET;
                        processVAdjust = false;
                    } else if (lastLineInVAdjust) {
                        c9Update = UpdateMode::RESET;
                        c4Update = UpdateMode::RESET;
                        //updateVideoOffset();
                        processVAdjust = false;
                    } else if (c9_rCounter >= regs[9]) {
                        // This covers C9 = R9 after the update.
                        c5Update = UpdateMode::INCREMENT;
                        c9Update = UpdateMode::RESET;
                    } else {
                        // This covers C9 == R9 before the update. (No update
                        // happened).
                        c5Update = UpdateMode::INCREMENT;
                        c9Update = UpdateMode::INCREMENT;
                    }
                    break;
            }
        }

        charAddress = lineAddress;

        if ((c4_vCounter == r7_vSyncPos) && (c9_rCounter == 0)) {  // Vertical Sync Position
            vSync = true;
            c3h_vSyncWidth = 0;
        }

    } else if (c0_hCounter == 1) {
        enableRasterCounter = true;
    } else if (c0_hCounter == 2) {
        switch (type) {
            case 0: // CRTC 0
                // When C0=2, the CRTC determines whether additional line management
                // should take place. If this additional management is activated,
                // then C4 will be incremented whatever the value of R4 for the next
                // C0=0. Otherwise, C4 and C9 will return to 0.
                if (processVAdjust) {
                    if (!r5_vAdjust) {
                        c9Update = UpdateMode::RESET;
                        processVAdjust = false;
                    }
                }
                break;
            default:
                break;
        }

    }


    if (c0_hCounter == r1_hDisplayed) {
        hDisplay = false;                   // Drawing border

        if (c9_rCounter == regs[9]) {
            lineAddress += r1_hDisplayed;
        }
    }

    // CRTC type 2, 3, 4: HSW in range 1..16. This block checks 1, 2, 3...
    // HSW = 0 gives 16 chars.
    if (hSync && type > 1) {
        c3l_hSyncWidth = (c3l_hSyncWidth + 1) & 0xF;
        if (c3l_hSyncWidth == r3l_hSyncWidth) {
            hSync = false;
            c3l_hSyncWidth = 0;
        }
    }

    if (c0_hCounter == r2_hSyncPos) {   // Horizontal Sync Position
        hSync = true;                       // HSYNC pulse
        c3l_hSyncWidth = 0;
    }

    // CRTC type 0, 1: HSW in range 0..15. This block checks 0, 1, 2...
    // HSW = 0 gives no HSYNC.
    if (hSync && type < 2) {
        if (c3l_hSyncWidth == r3l_hSyncWidth) {
            hSync = false;
            c3l_hSyncWidth = 0;
        } else if (type == 1 && r3l_hSyncWidth == 0) {
            hSync = false;
            c3l_hSyncWidth = 0;
        } else {
            c3l_hSyncWidth = (c3l_hSyncWidth + 1) & 0xF;
        }
    }

    // All CRTC types consider 1, 2, 3... 16.
    // CRTC types 1 and 2 have this setting fixed to 16 lines.
    // This block must be placed here so VSW = 1 actually does 1 line.
    if (vSync && (c0_hCounter == vSyncOffset)) {
        c3h_vSyncWidth = (c3h_vSyncWidth + 1) & 0xF;
        if (c3h_vSyncWidth == r3h_vSyncWidth) {
            vSync = false;
            c3h_vSyncWidth = 0;
        }
    }

    if ((c4_vCounter == r7_vSyncPos)
            && (c9_rCounter == 0) && (c0_hCounter == vSyncOffset)) {  // Vertical Sync Position
        vSync = true;
        c3h_vSyncWidth = 0;
    }

    if (c0_hCounter) charAddress++;
    pageAddress = (charAddress & 0x3000) << 2;
    byteAddress = pageAddress | ((c9_rCounter & 7) << 11) | ((charAddress & 0x3FF) << 1);
    dispEn = hDisplay && vDisplay;

    r9_rMaxAddress = regs[9];
}

void CRTC::updateC4() {

    switch (c4Update) {
        case UpdateMode::RESET:
            c4_vCounter = 0;
            vDisplay = true;
            updateVideoOffset();
            break;

        case UpdateMode::INCREMENT:
            c4_vCounter = (c4_vCounter + 1) & 0x7F;
            if (!c4_vCounter) {
                vDisplay = true;
                updateVideoOffset();
            }
            if (c4_vCounter == r6_vDisplayed) {
                vDisplay = false;
            }
            break;
        default:
            break;
    }

    if (c4_vCounter == 0 && type == 1) {
        updateVideoOffset();
    }

    c4Update = UpdateMode::KEEP;
}

void CRTC::updateC9() {

    switch (c9Update) {
        case UpdateMode::RESET:
            c9_rCounter = 0;
            break;
        case UpdateMode::INCREMENT:
            c9_rCounter = (c9_rCounter + 1) & 0x1F;
            break;
        default:
            break;
    }

    c9Update = UpdateMode::KEEP;
}

void CRTC::updateC5() {

    switch (c5Update) {
        case UpdateMode::RESET:
            c5_aCounter = 0;
            break;
        case UpdateMode::INCREMENT:
            c5_aCounter = (c5_aCounter + 1) & 0x1F;
            break;
        default:
            break;
    }

    c5Update = UpdateMode::KEEP;
}

void CRTC::updateVideoOffset() {

    lineAddress = (regs[12] & 0x3F) << 8 | regs[13];
    videoOffsetUpdated = true;
}

void CRTC::reset() {

    for (size_t ii = 0; ii < 31; ++ii) {
        regs[ii] = 0x00;
    }
    oddField = true;
}
// vim: et:sw=4:ts=4
