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
            byte = vDisplayed ? 0x20 : 0x00;
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
                    byte = vCounter;
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
                hTotal = regs[0];
                break;
            case 1: // Horizontal Displayed.
                hDisplayed = regs[1];
#ifdef DEBUGCRTC
                cout << "Update HDisp(" << static_cast<uint32_t>(hDisplayed)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 2: // Horizontal Sync Position.
                hsPos = regs[2];
                break;
            case 3: // Horizontal & Vertical Sync Width.
                vswMax = (regs[3] & 0xF0) >> 4;
                hswMax = (regs[3] & 0x0F);
                switch (type) {
                    case 1: // Type 1 (UM6845R):
                        // VSW is ignored. VSYNC is fixed to 16 lines.
                        vswMax = 0x0;
                        break;
                    case 2: // Type 2 (MC6845):
                        // VSW is ignored. VSYNC is fixed to 16 lines.
                        vswMax = 0x0;
                        break;
                    default:
                        break;
                }
                break;
            case 4: // Vertical total.
                vTotal = regs[4];
                vTotalUpdated = true;
#ifdef DEBUGCRTC
                cout << "Update VTotal(" << static_cast<uint32_t>(vTotal)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 5: // Vertical adjust.
                vAdjust = regs[5];
#ifdef DEBUGCRTC
                cout << "Update VAdjust(" << static_cast<uint32_t>(vAdjust)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 6: // Vertical displayed.
                vDisplayed = regs[6];
#ifdef DEBUGCRTC
                cout << "Update VDisplayed(" << static_cast<uint32_t>(vDisplayed)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 7: // Vertical Sync Position.
                vsPos = regs[7];
                if (vCounter == vsPos) {
                    vSync = true;
                    vswCounter = 0;
                }
                break;
            case 12:    // fall-through
            case 13:
#ifdef DEBUGCRTC
                cout << "Update base address(" << regs[13] + (regs[12] & 0x3F) * 0x100
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
                break;
#endif
            default:
                break;
        }
    }

    double base = (1000000.0 / 57.5);
    vSyncSeparation = base / (hTotal ? hTotal : 64);
}

void CRTC::clock() {

    updateVideoOffset = false;

    // If Horizontal Character Counter (HCC, C0) matches Horizontal Total (R0),
    // it returns to 0. Otherwise, it is incremented.
    if (hCounter == hTotal) {
        hCounter = 0;
    } else {
        ++hCounter;
    }

    // Some processing occurs on different values of HCC/C0.
    if (hCounter == 0) {
        // Image is displayed again for this line.
        hDisplay = true;

        // Check C9 == R9.
        bool lastLineInCharRowBefore = (rCounter == rMax);
        bool lastLineInCharRowAfter = (rCounter == regs[9]);
        bool lastLineInCharRow = lastLineInCharRowBefore || lastLineInCharRowAfter;
        // Check C4 == R4.
        bool lastCharRow = (vCounter == vTotal);

        if (!processVAdjust) {
            switch (type) {
                case 0: // CRTC Type 0:
                    if (lastCharRow) { // C4 == R4
                        if (lastLineInCharRowBefore) {
                            // C9 and C4 go to 0.
                            // Video offset is updated.
                            // This is the "last line" case, hence if C5 != 0
                            // the vertical adjustment phase is entered.
                            rCounter = 0;
                            if (vAdjust) {
                                vCounter = (vCounter + 1) & 0x7F;
                                processVAdjust = true;
                            } else {
                                vCounter = 0;
                                updateVideoOffset = true;
                            }
                        } else if (lastLineInCharRowAfter) {
                            // C9 goes to 0, C4 is incremented and overflows.
                            vCounter = (vCounter + 1) & 0x7F;
                            rCounter = 0;
                        } else {
                            // C9 is incremented.
                            // Video offset is not updated.
                            rCounter = (rCounter + 1) & 0x1F;
                        }
                    } else { // General behaviour (C4 != R4)
                        if (lastLineInCharRowAfter) {
                            // This covers C9 = R9 after the update.
                            vCounter = (vCounter + 1) & 0x7F;
                            rCounter = 0;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            rCounter = (rCounter + 1) & 0x1F;
                        }
                    }
                    break;

                case 1: // CRTC Type 1:
                    if (lastCharRow) { // C4 == R4
                        if (lastLineInCharRowAfter) {
                            // C9 goes to 0.
                            // This is the "last line" case, hence if C5 != 0
                            // the vertical adjustment phase is entered.
                            rCounter = 0;
                            if (vAdjust) {
                                // Set C5=0, increment C4.
                                vaCounter = 0;
                                vCounter = (vCounter + 1) & 0x7F;
                                processVAdjust = true;
                            } else {
                                // C4 goes to 0.
                                // Video offset is updated.
                                vCounter = 0;
                                updateVideoOffset = true;
                            }
                        } else {
                            // C9 is incremented.
                            // Video offset is updated if C4 == 0.
                            rCounter = (rCounter + 1) & 0x1F;
                            updateVideoOffset = (vCounter == 0);
                        }
                    } else { // General behaviour (C4 != R4)
                        if (lastLineInCharRowAfter) {
                            // This covers C9 = R9 after the update.
                            rCounter = 0;
                            vCounter = (vCounter + 1) & 0x7F;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            rCounter = (rCounter + 1) & 0x1F;
                            updateVideoOffset = (vCounter == 0);
                        }
                    }
                    break;

                case 2: // CRTC Type 2:
                    if (lastCharRow) { // C4 == R4
                        if (vAdjust) {
                            // CRTC 0,1,2 increment VCC/C4. CRTC 3,4 dont.
                            vCounter = (vCounter + 1) & 0x7F;
                            processVAdjust = true;
                        } else if (lastLineInCharRow) {
                            // C9 and C4 go to 0.
                            // Video offset is updated if C0 <= R1.
                            // This is the "last line" case, hence if C5 != 0
                            // the vertical adjustment phase is entered.
                            rCounter = 0;
                            if (vAdjust) {
                                // Set C5=0, increment C4.
                                vaCounter = 0;
                                vCounter = (vCounter + 1) & 0x7F;
                                processVAdjust = true;
                            } else {
                                vCounter = 0;
                                updateVideoOffset = (hCounter <= hDisplayed);
                            }
                            // TODO: Check for HSYNC cancellation.
                            // It applies to the BEFORE case, when C4 == R4 == 0.
                        } else {
                            // C9 is incremented.
                            // Video offset is not updated.
                            rCounter = (rCounter + 1) & 0x1F;
                        }
                    } else { // General behaviour (C4 != R4)
                        if (lastLineInCharRowAfter) {
                            // This covers C9 = R9 after the update.
                            rCounter = 0;
                            vCounter = (vCounter + 1) & 0x7F;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            rCounter = (rCounter + 1) & 0x1F;
                        }
                    }
                    break;

                default: // CRTC 3 & 4.
                    if (lastCharRow) { // C4 == R4
                        if (rCounter >= regs[9]) {
                            rCounter = 0;
                            if (vAdjust) {
                                vaCounter = 0;
                                processVAdjust = true;
                            } else {
                                vCounter = 0; 
                                updateVideoOffset = true;
                            }
                        } else {
                            // C9 is incremented.
                            // Video offset is not updated.
                            rCounter = (rCounter + 1) & 0x1F;
                        } 
                    } else { // General behaviour (C4 != R4).
                        if (rCounter >= regs[9]) {
                            // This covers C9 = R9 after the update.
                            rCounter = 0;
                            vCounter = (vCounter + 1) & 0x7F;
                        } else {
                            // This covers C9 == R9 before the update. (No update
                            // happened).
                            rCounter = (rCounter + 1) & 0x1F;
                        }
                    }
                    break;
            }
        } else { // processVAdjust == true
            // Check C9 == R5.
            bool lastLineInVAdjustRC = (rCounter == vAdjust);
            bool lastLineInVAdjustAC = (vaCounter == vAdjust);

            switch (type) {
                case 0: // CRTC 0.
                    if (lastLineInVAdjustRC) {
                        // Last line of the screen.
                        rCounter = vCounter = 0;
                        processVAdjust = false;
                        updateVideoOffset = true;
                    } else {
                        rCounter = (rCounter + 1) & 0x1F;
                    }
                    break;

                case 1: // CRTC 1, 2.
                case 2: // fall-through
                    if (!vAdjust) {
                        rCounter = 0;
                        processVAdjust = false;
                    } else if (lastLineInVAdjustAC) {
                        // Last line of the screen.
                        rCounter = vCounter = 0;
                        processVAdjust = false;
                        updateVideoOffset = true;
                    } else if (lastLineInCharRowAfter) {
                        // This covers C9 = R9 after the update.
                        rCounter = 0;
                        vaCounter = (vaCounter + 1) & 0x1F;
                        vCounter = (vCounter + 1) & 0x7F;
                    } else {
                        // This covers C9 == R9 before the update. (No update
                        // happened).
                        rCounter = (rCounter + 1) & 0x1F;
                        vaCounter = (vaCounter + 1) & 0x1F;
                    }
                    break;

                default: // CRTC 3, 4.
                    if (lastLineInVAdjustAC) {
                        rCounter = vCounter = 0;
                        processVAdjust = false;
                        updateVideoOffset = true;
                    } else if (rCounter >= regs[9]) {
                        // This covers C9 = R9 after the update.
                        rCounter = 0;
                        vaCounter = (vaCounter + 1) & 0x1F;
                    } else {
                        // This covers C9 == R9 before the update. (No update
                        // happened).
                        rCounter = (rCounter + 1) & 0x1F;
                        vaCounter = (vaCounter + 1) & 0x1F;
                    }
                    break;
            }
        }
    } else if (hCounter == 1) {
    } else if (hCounter == 2) {
    }

    if (hCounter == hDisplayed) {   // Horizontal Displayed
        hDisplay = false;                   // Drawing border

        if (rCounter == rMax) {
            lineAddress += hDisplayed;
        }

        // Base address is updated on VCC=0 (CRTC 1) or VCC=0 and VLC=0 (other)
        if (updateVideoOffset) {
            lineAddress = ((regs[12] & 0x3F) << 8) | regs[13];
#ifdef DEBUGCRTC
            cout << "Updated line address on top area: " << lineAddress << endl;
#endif
        }
    }

    if (vDispOff) {  // Vertical Displayed
        vDispOff = false;
        vDisplay = false;
    }

    if (hCounter == hDisplayed) {
        hDispOff = true;
    }

    if (hDispOff) {   // Horizontal Displayed
        hDispOff = false;
        hDisplay = false;                   // Drawing border

        if (rCounter == rMax - 1) {
            lineAddress += hDisplayed;
        }
    }

    // CRTC type 2, 3, 4: HSW in range 1..16. This block checks 1, 2, 3...
    // HSW = 0 gives 16 chars.
    if (hSync && type > 1) {
        hswCounter = (hswCounter + 1) & 0xF;
        if (hswCounter == hswMax) {
            hSync = false;
            hswCounter = 0;
        }
    }

    if (hCounter == hsPos) {   // Horizontal Sync Position
        hSync = true;                       // HSYNC pulse
        hswCounter = 0;
    }

    // CRTC type 0, 1: HSW in range 0..15. This block checks 0, 1, 2...
    // HSW = 0 gives no HSYNC.
    if (hSync && type < 2) {
        if (hswCounter == hswMax) {
            hSync = false;
            hswCounter = 0;
        } else {
            hswCounter = (hswCounter + 1) & 0xF;
        }
    }

    // All CRTC types consider 1, 2, 3... 16.
    // CRTC types 1 and 2 have this setting fixed to 16 lines.
    // This block must be placed here so VSW = 1 actually does 1 line.
    if (vSync && (hCounter == vSyncOffset)) {
        vswCounter = (vswCounter + 1) & 0xF;
        if (vswCounter == vswMax) {
            vSync = false;
            vswCounter = 0;
        }
    }

    if ((vCounter == vsPos) && (rCounter == 0) && (hCounter == vSyncOffset)) {  // Vertical Sync Position
        vSync = true;
        vswCounter = 0;
    }

    charAddress = lineAddress + hCounter;
    pageAddress = (charAddress & 0x3000) << 2;
    byteAddress = pageAddress | ((rCounter & 7) << 11) | ((charAddress & 0x3FF) << 1);
    dispEn = hDisplay && vDisplay;

    rMax = regs[9];
}

void CRTC::reset() {

    for (size_t ii = 0; ii < 31; ++ii) {
        regs[ii] = 0x00;
    }
    oddField = true;
}
// vim: et:sw=4:ts=4
