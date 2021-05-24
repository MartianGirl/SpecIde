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

#include <iostream>
using namespace std;

CRTC::CRTC(uint_fast8_t type) :
    type(type),
    index(0),
    regs{
        //0x3F, 0x28, 0x2E, 0x8E, 0x26, 0x00, 0x19, 0x1A,
        //0x00, 0x07, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, static_cast<uint_fast8_t>((type == 1) ? 0xFF : 0x00)},
    dirs{
        AccessType::CRTC_WO,    // Horizontal Total (WO)
        AccessType::CRTC_WO,    // Horizontal Displayed (WO)
        AccessType::CRTC_WO,    // Horizontal Sync Position (WO)
        AccessType::CRTC_WO,    // Horizontal & Vertical Sync Width (WO)
        AccessType::CRTC_WO,    // Vertical Total (WO)
        AccessType::CRTC_WO,    // Vertical Total Adjust (WO)
        AccessType::CRTC_WO,    // Vertical Displayed (WO)
        AccessType::CRTC_WO,    // Vertical Sync Position (WO)
        AccessType::CRTC_WO,    // Interlace & Skey (WO)
        AccessType::CRTC_WO,    // Maximum Raster Address (WO)
        AccessType::CRTC_RW,    // Cursor Start Raster (RW)
        AccessType::CRTC_RW,    // Cursor End Raster (RW)
        (type == 0 || type == 3 || type == 4)
            ? AccessType::CRTC_RW : AccessType::CRTC_WO,    // Display Start Address (High byte)
        (type == 0 || type == 3 || type == 4)
            ? AccessType::CRTC_RW : AccessType::CRTC_WO,    // Display Start Address (Low byte)
        AccessType::CRTC_RW,    // Cursor Address (High byte)
        AccessType::CRTC_RW,    // Cursor Address (Low byte)
        AccessType::CRTC_RO,    // Light Pen Address (High byte)
        AccessType::CRTC_RO,    // Light Pen Address (Low byte)
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
        regs[index] = byte;
    }

    if (index == 3) {
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

    return 0xFF;
}

uint_fast8_t CRTC::rdRegister() {

    return (dirs[index] == AccessType::CRTC_RO
            || dirs[index] == AccessType::CRTC_RW)
        ? regs[index] : 0x00;
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

            lineAddress = ((regs[12] & 0x3F) * 0x100 + regs[13]) + (regs[1] * vCounter);

            if (vCounter == regs[6]) {  // Vertical Displayed
                vDisplay = false;
            }

            if (vCounter == regs[7]) {  // Vertical Sync Position
                vSync = true;
            }
        }

        scanAddress = ((rCounter & 7) << 11) | ((lineAddress & 0x3FF) << 1);
        pageAddress = lineAddress >> 12;

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
        if (hswCounter == hswMax) {    // Horizontal Sync Width
            hSync = false;
            hswCounter = 0;
        }
    }

    byteAddress = scanAddress + hCounter * 2;
    dispEn = hDisplay && vDisplay;
}
// vim: et:sw=4:ts=4
