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

#undef DEBUGLOG
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
                hTotal = regs[0] + 1;
                break;
            case 1: // Horizontal Displayed.
                hDisplayed = regs[1];
#ifdef DEBUGLOG
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
                vTotal = regs[4] + 1;
                vTotalUpdated = true;
#ifdef DEBUGLOG
                cout << "Update VTotal(" << static_cast<uint32_t>(vTotal)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 5: // Vertical adjust.
                vAdjust = regs[5];
#ifdef DEBUGLOG
                cout << "Update VAdjust(" << static_cast<uint32_t>(vAdjust)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 6: // Vertical displayed.
                vDisplayed = regs[6];
#ifdef DEBUGLOG
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
            case 9: // Max Raster Address.
                rMax = regs[9] + 1;
                rMaxUpdated = true;
#ifdef DEBUGLOG
                cout << "Update RMax(" << static_cast<uint32_t>(rMax)
                    << ") at VCC=" << static_cast<uint32_t>(vCounter)
                    << " VLC=" << static_cast<uint32_t>(rCounter)
                    << " VAD=" << static_cast<uint32_t>(vaCounter)
                    << " HCC=" << static_cast<uint32_t>(hCounter) << endl;
#endif
                break;
            case 12:    // fall-through
            case 13:
                updateLineAddress = (vCounter == 0) && ((rCounter == 0) || type == 1);
#ifdef DEBUGLOG
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

    maxScans = vTotal * rMax + vAdjust;

    double base = (1000000.0 / 57.5);
    vSyncSeparation = base / (hTotal ? hTotal : 64);
}

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

void CRTC::clock() {

    // Counters:
    // - Character counter (aka Horizontal Counter)
    //      Compared to R0 (Horizontal Total Register)
    //          Set -> Skew control -> DISPTMG
    //          Reset Character counter (MR)
    //          Clock Raster counter
    //          Clock Vertical Sync Width Counter
    //          Affect Linear Address Generator
    //          Affect Interlace Control
    //
    //      Compared to R1 (Horizontal Displayed Register)
    //          Reset -> Skew control -> DISPTMG
    //          Reset -> Linear Address Generator
    //      Compared to R2 (Horizontal Sync Position Register)
    //          Set HSYNC
    //          Enable Horizontal Sync Width Counter
    //      Compared to R0 / 2
    //          Affect Interlace Control

    // Here increment Raster Counter, Vertical Sync Width Counter
    // Horizontal counter is incremented with each tick.
    ++hCounter;

    // On HCC=1 the conditions for End Of Frame and End Of Row are checked.
    if (hCounter == 1) {
#ifdef DEBUGLOG
        cout << "Position: VCC=" << static_cast<uint32_t>(vCounter)
            << " VLC=" << static_cast<uint32_t>(rCounter)
            << " VAD=" << static_cast<uint32_t>(vaCounter)
            << " HCC=" << static_cast<uint32_t>(hCounter)
            << " HSync=" << static_cast<uint32_t>(hsPos)
            << " HDisp=" << static_cast<uint32_t>(hDisplayed)
            << " HTotal=" << static_cast<uint32_t>(hTotal)
            << " VDisp=" << static_cast<uint32_t>(vDisplayed)
            << " Address=" << lineAddress << endl;
#endif
        nextRCounter = (rCounter + 1) & 0x1F;
        nextVCounter = (vCounter + 1) & 0x7F;
        nextACounter = (vaCounter + 1) & 0x1F;

        // If we entered adjust area, we are no longer considering VCC/VLC
        // counters.
        bool finishFrameOnAdjust = processVAdjust && (nextACounter == vAdjust);
        bool finishFrameOnRowEnd = (nextVCounter == vTotal) && (nextRCounter == rMax);
        finishFrameAttempt = finishFrameOnAdjust || finishFrameOnRowEnd;
#ifdef DEBUGLOG
        if (finishFrameOnAdjust) {
            cout << "Attempt finish frame (adjust) at VCC=" << static_cast<uint32_t>(vCounter);
            cout << " VLC=" << static_cast<uint32_t>(rCounter);
            cout << " Adjust=" << static_cast<uint32_t>(vAdjust) << endl;
        }
        if (finishFrameOnRowEnd) {
            cout << "Attempt finish frame (row end) at VCC=" << static_cast<uint32_t>(vCounter);
            cout << " VLC=" << static_cast<uint32_t>(rCounter);
            cout << " Adjust=" << static_cast<uint32_t>(vAdjust) << endl;
        }
#endif
        finishRow = (nextRCounter == rMax);
    }

    // On HCC=2 the conditions for entering Vertical Adjustment are checked.
    if (hCounter == 2) {
        enterVAdjust = (vCounter == vTotal - 1) && (rCounter == rMax - 1) && (vAdjust != 0);
#ifdef DEBUGLOG
        if (enterVAdjust) {
            cout << "Entering vAdjust at VCC=" << static_cast<uint32_t>(vCounter);
            cout << " VLC=" << static_cast<uint32_t>(rCounter) << endl;
        }
#endif
        finishFrame = finishFrameAttempt && !enterVAdjust;
    }

    // I'm going to leave this here for the moment. If there are no further cases
    // where endOfScan out of sequence is needed, I'll merge this condition with
    // the following if block.
    if (hCounter == hTotal) {
        endOfScan = true;
    }

    if (endOfScan) {    // Horizontal Total marks the end of a scan.
        hCounter = 0;               // Reset Horizontal Counter.
        hDisplay = true;            // Start drawing screen.

        if (processVAdjust) {
            vaCounter = (vaCounter + 1) & 0x1F;
        } else {
            if (vTotalUpdated || rMaxUpdated) {
                // Abort end of frame on VLC=0 if vTotal is set so VCC!=VTotal.
                if (rCounter == 0 && vCounter == 0 && nextVCounter != vTotal) {
                    finishFrame = false;
                }
                // Updated R4 or R9 may trigger a frame end.
                if (nextVCounter == vTotal && (finishRow || nextRCounter == rMax)) {
                    finishFrame = true;
                }
                // If frame is not finished, maybe row is...
                if (!finishFrame) {
                    finishRow = (nextRCounter == rMax);
                }
            }

            rCounter = (rCounter + 1) & 0x1F;   // Increment raster counter.
        }

        if (enterVAdjust) {
            processVAdjust = true;
        }
        // finishRow marks the end of row at the end of this scan, from HCC == 1.
        // endOfRow marks the actual end of this scan, on HCC == HTotal.
        endOfFrame = finishFrame;
        endOfRow = finishRow && !endOfFrame;
        finishRow = finishFrame = vTotalUpdated = rMaxUpdated = false;

#ifdef DEBUGLOG
        if (rCounter > rMax) {
            cout << "rCounter out of limit at VCC=" << static_cast<uint32_t>(vCounter);
            cout << " VLC=" << static_cast<uint32_t>(rCounter) << endl;
        }
#endif
        // Base address is updated on VCC=0 (CRTC 1) or VCC=0 and VLC=0 (other)
        if (updateLineAddress) {
            lineAddress = ((regs[12] & 0x3F) << 8) | regs[13];
            updateLineAddress = false;
#ifdef DEBUGLOG
            cout << "Updated line address on top area: " << lineAddress << endl;
#endif
        }
    }

    if (endOfRow) { // Maximum Raster Address
        rCounter = 0;           // Reset Raster Counter
        vaCounter = 0;

        vCounter = (vCounter + 1) & 0x7F;
#ifdef DEBUGLOG
        if (vCounter > vTotal) {
            cout << "VCC out of range VCC=" << static_cast<uint32_t>(vCounter) << endl;
        }
#endif
        vDispOff = (vCounter == vDisplayed);
        oddField = !oddField;
    }

    if (endOfFrame) {
#ifdef DEBUGLOG
        cout << "Finish frame at VCC=" << static_cast<uint32_t>(vCounter);
        cout << " VLC=" << static_cast<uint32_t>(rCounter) << endl;
#endif
        vCounter = 0;
        rCounter = 0;
        vaCounter = 0;
        processVAdjust = false;

        vDisplay = true;
        if (vDisplayed == 0) vDispOff = true;
        // Vertical Total marks the end of a frame, but we also must
        // account for Vertical Total Adjustment
        interlace = regs[8] & 0x3;
        if (interlace & 0x1) {
            oddField = !oddField;
            vSyncOffset = !oddField ? (regs[0] / 2 + 1) : 0;
        } else {
            oddField = true;
            vSyncOffset = 0;
        }

        if (oddField) {
            lineAddress = ((regs[12] & 0x3F) << 8) | regs[13];
#ifdef DEBUGLOG
            cout << "Updated line address on end of frame: " << lineAddress << endl;
#endif
        }
    }

    if (vDispOff) {  // Vertical Displayed
        vDispOff = false;
        vDisplay = false;
    }

    endOfScan = false;
    endOfRow = false;
    endOfFrame = false;

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

}

void CRTC::reset() {

    for (size_t ii = 0; ii < 31; ++ii) {
        regs[ii] = 0x00;
    }
    oddField = true;
}
// vim: et:sw=4:ts=4
