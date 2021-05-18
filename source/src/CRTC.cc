/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
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

CRTC::CRTC(uint_fast8_t type) :
    type(type),
    address(0),
    regs{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ((type == 1) ? 0xFF : 0x00)},
    dirs{
        CRTC_WO,    // Horizontal Total (WO)
        CRTC_WO,    // Horizontal Displayed (WO)
        CRTC_WO,    // Horizontal Sync Position (WO)
        CRTC_WO,    // Horizontal & Vertical Sync Width (WO)
        CRTC_WO,    // Vertical Total (WO)
        CRTC_WO,    // Vertical Total Adjust (WO)
        CRTC_WO,    // Vertical Displayed (WO)
        CRTC_WO,    // Vertical Sync Position (WO)
        CRTC_WO,    // Interlace & Skey (WO)
        CRTC_WO,    // Maximum Raster Address (WO)
        CRTC_RW,    // Cursor Start Raster (RW)
        CRTC_RW,    // Cursor End Raster (RW)
        (type == 0 || type == 3 || type == 4)
            ? CRTC_RW : CRTC_WO,    // Display Start Address (High byte)
        (type == 0 || type == 3 || type == 4)
            ? CRTC_RW : CRTC_WO,    // Display Start Address (Low byte)
        CRTC_RW,    // Cursor Address (High byte)
        CRTC_RW,    // Cursor Address (Low byte)
        CRTC_RO,    // Light Pen Address (High byte)
        CRTC_RO,    // Light Pen Address (Low byte)
        CRTC_RO, CRTC_RO,   // All the remaining addresses are unused.
        CRTC_RO, CRTC_RO, CRTC_RO, CRTC_RO,
        CRTC_RO, CRTC_RO, CRTC_RO, CRTC_RO,
        CRTC_RO, CRTC_RO, CRTC_RO, CRTC_RO} {}

void CRTC::wrAddress(uint_fast8_t byte) {

    address = byte & 0x1F;
}

void CRTC::wrRegister(uint_fast8_t byte) {

    if (dirs[address] == CRTC_WO || dirs[address] == CRTC_RW) {
        regs[address] = byte;
    }

    if (address == 0x03) {
        hswMax = byte & 0x0F;
        switch (type) {
            case 0:
                vswMax = 0x10;
                break;
            case 1:
                vswMax = (byte & 0xF0) >> 8;
                break;
            default:
                vswMax = (byte & 0xF0);
                break;
        }
    }
}

uint_fast8_t CRTC::rdStatus() {

    if (type == 1) {
    }
}

uint_fast8_t CRTC::rdRegister() {

    return (dirs[address] == CRTC_RO || dirs[address] == CRTC_RW)
        ? regs[address] : 0x00;
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

    // Horizontal counter is incremented with each tick
    ++hCounter;

    // This is for interlace control
    hh = (hCounter > (regs[0] >> 1));

    // Here increment Raster Counter, Vertical Sync Width Counter
    if (hCounter > regs[0]) {   // Horizontal Total marks the end of a scan
        hCounter = 0;               // Reset Horizontal Counter 
        hDisplay = true;            // Drawing screen

        // Increment raster counter and check
        rCounter = (rCounter + 1) & 0x1F;
        if (rCounter > regs[9]) {   // Maximum Raster Address
            rCounter = 0;               // Reset Raster Counter

            vCounter = (vCounter + 1) & 0x7F;
            // Vertical Total marks the end of a frame, but we also must
            // account for Vertical Total Adjustment
            if (vCounter > regs[4] && rCounter == regs[5]) {
                vCounter = 0;
                rCounter = 0;
                vDisplay = true;
            }

            if (vCounter == regs[6]) {  // Vertical Displayed
                vDisplay = false;
            }

            if (vCounter == regs[7]) {  // Vertical Sync Position
                vSync = true;
            }

            updateLineAddress();
        }

        if (vSync) {
            ++vswCounter;
            if (vswCounter == vswMax) {
                vSync = false;
                vswCounter = 0;
            }
        }
    }

    if (hCounter == regs[1]) {   // Horizontal Displayed
        hDisplay = false;                   // Drawing border
    }

    if (hCounter == regs[2]) {   // Horizontal Sync Position
        hSync = true;                       // HSYNC pulse
    }

    if (hSync) {    // Horizontal Sync Width is incremented during HSYNC pulse
        ++hswCounter;
        if (hswCounter == regs[3]) {    // Horizontal Sync Width
            hSync = false;
            hswCounter = 0;
        }
    }

    byteAddress = scanAddress + hCounter;
}

void updateScanAddress() {

    scanAddress = regs[13] * 0x100 + regs[12]               // Video base address
        + (vCounter * (regs[9] + 1) + rCounter) * regs[1];  // Scan offset
}
// vim: et:sw=4:ts=4
