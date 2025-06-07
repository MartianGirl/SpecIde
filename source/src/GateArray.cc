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

#include "GateArray.h"
#include "Utils.h"

#include <iostream>

using namespace std;

#if SPECIDE_BYTE_ORDER == 1
uint32_t GateArray::palette[CPC_NUM_PALETTES][32] = {
    // Colour monitor
    {
        0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF, 0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
        0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
        0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF, 0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
        0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF, 0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF
    },
    // Black and white monitor
    {
        0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF, 0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
        0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
        0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF, 0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
        0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF, 0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF
    },
    // Green phosphor monitor
    {
        0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF, 0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
        0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
        0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF, 0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
        0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF, 0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF
    },
    // Custom palette
    {
        0x7F7F7FFF, 0x7F7F7FFF, 0x00FF7FFF, 0xFFFF7FFF, 0x00007FFF, 0xFF007FFF, 0x007F7FFF, 0xFF7F7FFF,
        0xFF007FFF, 0xFFFF7FFF, 0xFFFF00FF, 0xFFFFFFFF, 0xFF0000FF, 0xFF00FFFF, 0xFF7F00FF, 0xFF7FFFFF,
        0x00007FFF, 0x00FF7FFF, 0x00FF00FF, 0x00FFFFFF, 0x000000FF, 0x0000FFFF, 0x007F00FF, 0x007FFFFF,
        0x7F007FFF, 0x7FFF7FFF, 0x7FFF00FF, 0x7FFFFFFF, 0x7F0000FF, 0x7F00FFFF, 0x7F7F00FF, 0x7F7FFFFF
    }
};
#else
uint32_t GateArray::palette[CPC_NUM_PALETTES][32] = {
    // Colour monitor
    {
        0xFF7F7F7F, 0xFF7F7F7F, 0xFF7FFF00, 0xFF7FFFFF, 0xFF7F0000, 0xFF7F00FF, 0xFF7F7F00, 0xFF7F7FFF,
        0xFF7F00FF, 0xFF7FFFFF, 0xFF00FFFF, 0xFFFFFFFF, 0xFF0000FF, 0xFFFF00FF, 0xFF007FFF, 0xFFFF7FFF,
        0xFF7F0000, 0xFF7FFF00, 0xFF00FF00, 0xFFFFFF00, 0xFF000000, 0xFFFF0000, 0xFF007F00, 0xFFFF7F00,
        0xFF7F007F, 0xFF7FFF7F, 0xFF00FF7F, 0xFFFFFF7F, 0xFF00007F, 0xFFFF007F, 0xFF007F7F, 0xFFFF7F7F
    },
    // Black and white monitor
    {
        0xFF7F7F7F, 0xFF7F7F7F, 0xFFBCBCBC, 0xFFD9D9D9, 0xFF262626, 0xFF434343, 0xFF717171, 0xFF8E8E8E,
        0xFF434343, 0xFFD9D9D9, 0xFFB3B3B3, 0xFFFFFFFF, 0xFF1D1D1D, 0xFF696969, 0xFF686868, 0xFFB4B4B4,
        0xFF262626, 0xFFBCBCBC, 0xFF969696, 0xFFE2E2E2, 0xFF000000, 0xFF4C4C4C, 0xFF4B4B4B, 0xFF979797,
        0xFF343434, 0xFFCACACA, 0xFFA4A4A4, 0xFFF0F0F0, 0xFF0E0E0E, 0xFF5B5B5B, 0xFF595959, 0xFFA5A5A5
    },
    // Green phosphor monitor
    {
        0xFF007F00, 0xFF007F00, 0xFF00BC00, 0xFF00D900, 0xFF002600, 0xFF004300, 0xFF007100, 0xFF008E00,
        0xFF004300, 0xFF00D900, 0xFF00B300, 0xFF00FF00, 0xFF001D00, 0xFF006900, 0xFF006800, 0xFF00B400,
        0xFF002600, 0xFF00BC00, 0xFF009600, 0xFF00E200, 0xFF000000, 0xFF004C00, 0xFF004B00, 0xFF009700,
        0xFF003400, 0xFF00CA00, 0xFF00A400, 0xFF00F000, 0xFF000E00, 0xFF005B00, 0xFF005900, 0xFF00A500
    },
    // Custom palette
    {
        0xFF7F7F7F, 0xFF7F7F7F, 0xFF7FFF00, 0xFF7FFFFF, 0xFF7F0000, 0xFF7F00FF, 0xFF7F7F00, 0xFF7F7FFF,
        0xFF7F00FF, 0xFF7FFFFF, 0xFF00FFFF, 0xFFFFFFFF, 0xFF0000FF, 0xFFFF00FF, 0xFF007FFF, 0xFFFF7FFF,
        0xFF7F0000, 0xFF7FFF00, 0xFF00FF00, 0xFFFFFF00, 0xFF000000, 0xFFFF0000, 0xFF007F00, 0xFFFF7F00,
        0xFF7F007F, 0xFF7FFF7F, 0xFF00FF7F, 0xFFFFFF7F, 0xFF00007F, 0xFFFF007F, 0xFF007F7F, 0xFFFF7F7F
    }
};
#endif

uint32_t GateArray::colours[32];

uint32_t GateArray::pixelsX1[X_SIZE * Y_SIZE / 2];
uint32_t GateArray::pixelsX2[X_SIZE * Y_SIZE];

void GateArray::setPalette(uint32_t type) {

    if (type < CPC_NUM_PALETTES) {
        for (size_t ii = 0; ii < 0x20; ++ii) {
            colours[ii] = palette[type][ii];
        }
    }

    fillAverageTable();
}

void GateArray::write(uint_fast8_t byte) {
    ioHasByte = true;
    ioByte = byte;
}

void GateArray::input(void) {

    // 11xx xxxx: RAM memory management (performed externally)
    switch (ioByte & 0xC0) {
        case 0x00:  // 00xx xxxx : Select pen
            selectPen(ioByte);
            break;

        case 0x40:  // 01xx xxxx : Select colour for selected pen
            selectColour(ioByte);
            break;

        case 0x80:  // 10xx xxxx : Select screen mode and ROM configuration
            selectScreenAndRom(ioByte);
            break;

        default:
            break;
    }
}

void GateArray::selectPen(uint_fast8_t byte) {

    pen = byte & 0x1F;
}

void GateArray::selectColour(uint_fast8_t byte) {

    if (pen & 0x10) {
        border = byte & 0x1F;
    } else {
        pens[pen & 0x0F] = byte & 0x1F;
    }
}

void GateArray::selectScreenAndRom(uint_fast8_t byte) {

    newMode = byte & 0x03;
    lowerRom = ((byte & 0x04) == 0);
    upperRom = ((byte & 0x08) == 0);

    // Delay interrupt.
    if (byte & 0x10) {
        z80_c |= SIGNAL_INT_;
        intCounter = 0;
    }
}

void GateArray::clock() {

    // 1. Sequencer works using a 8-bit shift register to generate the
    // following sequence:
    // 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
    // 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F
    //
    // The above sequence gets restarted to 0xFF whenever the Z80 acknowledges
    // an interrupt (M1_, IORQ_ and RD_ are low, RESET is high).
    //
    // The register is clocked at 16MHz.
    //
    // 2. From here, we derive the following signals:
    //
    // PHI_ (4 MHz clock)
    // (S1 xor S3) or (S5 xor S7), delayed one clock (has digits 378C)
    // true, false, false, true, true, false, false, true
    // true, false, false, true, true, false, false, true
    //
    // CASAD_
    // Same, but one clock after
    // false, true, true, false, false, false, false, false
    // false, false, false, true, true, false, false, false
    // 
    // CPU_
    // not (S1 and not S7)
    // true, true, true, true, true, true, true, true,
    // true, true, false, false, false, false, false, false
    //
    // S3 and not S6
    // false, false, false, false, false, false, false, false
    // false, false, false, false, true, true, true, false
    //
    // CASAD
    // true, false, false, true, true, true, true, true
    // true, true, true, false, false, true, true, true
    //
    // READY
    // (CASAD and READY) or (S3 and not S6)
    // true, false, false, false, false, false, false, false
    // false, false, false, false, true, true, true, true
    //
    //
    // CCLK_
    // S2 nor S5
    // false, false, false, false, false, false, true, true,
    // true, true, true, false, false, false, false, false
    //
    // (S0 and S5 and RD_)
    // RD_, false, false, false, false, false, false, false,
    // false, false, false, false, false, false, RD_, RD_
    //
    // MWE_
    // not (S0 and S5 and RD_)
    // !RD_, true, true, true, true, true, true, true
    // true, true, true, true, true, true, !RD, !RD
    //
    // 244E
    // not (S2 and S3 and not IORQ_)
    // IORQ_, IORQ_, IORQ_, true, true, true, true, true,
    // true, true, true, true, IORQ_, IORQ_, IORQ_, IORQ_
    //
    // INKEN (Gate Array enable)
    // (S0 and S7)
    // true, false, false, false, false, false, false, false
    // false, false, false, false, false, false, false, false
    //
    //
    // (not S4 and S5)
    // false, false, false, false, false, true, false, false
    // false, false, false, false, false, false, false, false
    //
    // (not S3 and S1)
    // false, false, false, false, false, false, false, false
    // false, false, true, true, false, false, false, false
    //
    // (S1 and S7)
    // true, true, false, false, false, false, false, false
    // false, false, false, false, false, false, false, false
    //
    // (not S4 and S5) or (not S3 and S1) or (S1 and S7)
    // true, true, false, false, false, true, false, false
    // false, false, true, true, false, false, false, false
    //
    // (delayed x1)
    // false, true, true, false, false, false, true, false,
    // false, false, false, true, true, false, false, false
    //
    // (delayed x2)
    // false, false, true, true, false, false, false, true,
    // false, false, false, false, true, true, false, false
    //
    // S2
    // true, true, true, false, false, false, false, false
    // false, false, false, true, true, true, true, true
    //
    // (not S4 or S5)
    // true, true, true, true, true, true, true, true,
    // true, true, true, true, true, false, true, true
    //
    // (not S4 or S5) or #MREQ (gated the refresh MREQ)
    // true, true, true, true, true, true, true, true
    // true, true, true, true, true, #MREQ, true, true
    //
    // signal above and S2
    // true, true, true, false, false, false, false, false
    // false, false, false, true, true, #MREQ, true, true
    //
    // ANG gate U712
    // #MREQ, true, true, false, false, false, false, false,
    // false, false, false, true, true, #MREQ, #MREQ, #MREQ,
    //
    // (delayed x1 or delayed x2)
    // false, true, true, true, false, false, true, true
    // false, false, false, true, true, true, false, false
    //
    // #CAS (delayed x1 or delayed x2 or U712)
    // #MREQ, true, true, true, false, false, true, true,
    // false, false, false, true, true, true, #MREQ, #MREQ
    //
    // RAS_
    // S0 and (not S2 or S6)
    // true, true, false, false, false, false, false, false
    // false, false, true, true, false, false, false, false
    //
    // 74LS153 multiplexers (0 = A15-A9,A0, 1 = A8-A1,
    // 2 = M13-M12, RA2-0, M9-M8-CCLK, 3 = M7-M0)
    // 2, 3, 3, 2, 2, 2, 2, 2,
    // 2, 2, 0, 1, 1, 0, 0, 0
    //
    // This means that the frame (starting at 0) is:
    // 0: nothing
    // 1: nothing - wait
    // 2: video row address latched - wait
    // 3: nothing - wait
    // 4: video col address latched - wait
    // 5: wait
    // 6: read video byte - wait
    // 7: wait
    // 8: video col address latched - wait
    // 9: wait
    // a: wait
    // b: read video byte - wait
    // c: cpu row address latched - go
    // d: nothng
    // e: cpu col address latched
    // f-0: get data

    // Increment state counter
    counter = (counter + 1) & 0x0F;

    switch (counter) {
        case 0x0:   // fall-through
        case 0x2:   // fall-through
        case 0x4:   // fall-through
        case 0x8:   // fall-through
        case 0xa:   // fall-through
        case 0xe:   // Right after CPU state change.
            intAcknowledge();
            break;

        case 0x1:   // READY falling edge. CPU clock rising edge.
            z80_c &= ~SIGNAL_WAIT_;
            if (ioHasByte) {
                input();
            }
            ioHasByte = false;
            break;
        case 0xc:   // READY rising edge. Right after CPU state change.
            intAcknowledge();
            z80_c |= SIGNAL_WAIT_;
            break;

        case 0x6:   // CAS_ and S3 rising edge. Right after CPU state change.
            intAcknowledge();
            dispen = crtc.dispEn;
            videoByte = d;
            break;
        case 0xb:   // CAS_ and S3 rising edge. CRTC is clocked here.
            dispen = crtc.dispEn;
            videoByte = d;

            // CRTC is clocked. Some values have updated.
            crtc.clock();

            generateInterrupts();
            updateVideoMode();
            updateBeam();

            hSync_d = crtc.hSync;
            vSync_d = crtc.vSync;
            break;
        default:
            break;
    }

    paint();
}

void GateArray::intAcknowledge() {

    static bool clearInt = false;

    if ((z80_c & (SIGNAL_M1_ | SIGNAL_IORQ_ | SIGNAL_INT_)) == 0) {
        clearInt = true;
    } else if (z80_c & SIGNAL_M1_) {
        clearInt = false;
    }

    if (clearInt) {
        intCounter &= 0x1F;
        z80_c |= SIGNAL_INT_;
    }
}

void GateArray::updateVideoMode() {

    // This cClk falling edge counter is reset when HSYNC is low,
    // and it counts up to 8. When it reaches 8, it stops and keeps
    // its value until HSYNC falls.
    // If cClkEdge is 0-3, the mode selection clock is high.
    static uint_fast32_t cClkCounter = 2;
    static bool trigger = false;

    if (trigger) {
        actMode = newMode;
        trigger = false;
    }

    if (crtc.hSync) {
        if (cClkCounter < 8) {
            if (++cClkCounter == 8) {
                trigger = true;
            }
        }
    } else {
        // Mode is updated on the falling edge of bit 4 of this counter.
        // If cClkCounter was already 8, we don't enter the if above.
        if (cClkCounter & 4) {
            trigger = true;
        }
        cClkCounter = 2;
    }
}

void GateArray::updateBeam() {

    // This function is invoked every time the CRTC is clocked, so each time
    // we count an horizontal character.
    ++charsFromHSync;
    ++charsInRaster;

    // Blanking should also be activated if hCounter < 28, but the picture
    // fits better the screen this way...
    blanking = crtc.hSync || hCounter < 0x1c;

    // Accept HSync only if longer than 2.
    if (crtc.hswCounter == 3 && charsFromHSync > 62) {
        hSyncAccepted = true;
        charsFromHSync = 0;
    }

    // The monitor can only accept VSyncs if they are within its vertical
    // frequency range. CRTC::vSyncSeparation is the number of scans for this
    // to happen.
    // The separation-between-VSyncs counter is reset when a VSync is accepted.
    if (crtc.vswCounter == 3 && rastersFromVSync >= crtc.vSyncSeparation) {
        vSyncAccepted = true;
        rastersFromVSync = 0;
    }

    if (xPos >= X_SIZE) {
        xPos = 0;   // Move beam to the left...
        xInc = 0;   // ...and keep it there... (for a moment)
    }

    // We consider the HSync pulse here, and we start moving the beam again...
    // But only if we were already in the left side.
    // (I'd say that this is redundant, in the way this function is written...)
    if (hSyncAccepted || (charsInRaster > 64 && !xPos)) {
        // Beam starting point depends on how long last raster was...
        xPos = 16 * ((charsInRaster > 64 && charsInRaster < 67)
                ? charsInRaster - 64 : 0);
        xInc = 1;
        ++rastersFromVSync;       // Consider the time of the horizontal sweep.

        // Clear chars in raster counter if there was an HSync.
        bool vSyncByOverflow = hSyncAccepted && charsInRaster > 127;
        if (hSyncAccepted) {
            charsInRaster = 0;
        }
        hSyncAccepted = false;  // We've already considered this HSync.

        // Vertical position (and scans-from-frame-start counter) are increased
        // only if HSync happens outside of a VSync pulse.
        // (This is regarding geommetry, not time!)
        if (!crtc.vSync || crtc.vswCounter < 2 || crtc.vswCounter > 5) {
            yPos += yInc;
        }

        // Retrace happens if the screen reaches the end, or if an VSync is
        // accepted (occurs within VFreq range). In this case, we position
        // the beam at the top of the screen and signal that we have a new frame.
        if (yPos >= (yInc * Y_SIZE / 2) || vSyncAccepted || vSyncByOverflow) {
#ifdef DEBUGCRTC
            cout << endl << "New frame at yPos=" << yPos << endl << endl;
#endif
            sync = (yPos > 0x7);
            if (!vSyncByOverflow) {
                for (size_t jj = yPos; jj < (yInc * Y_SIZE / 2); jj += yInc) {
                    for (size_t ii = 0; ii < X_SIZE; ++ii) {
                        switch (scanlines) {
                            case 1:
                                pixelsX2[((jj + frame) * X_SIZE) + ii] = colours[20];
                                break;
                            case 2:
                                pixelsX1[(jj * X_SIZE) + ii] = colours[20];
                                break;
                            case 3:
                                pixelsX2[(jj * X_SIZE) + ii] = colours[20];
                                pixelsX2[((jj + 1) * X_SIZE) + ii] = colours[20];
                                break;
                            default:
                                pixelsX1[(jj * X_SIZE) + ii] = colours[20];
                                break;
                        }
                    }
                }
            }
            yPos = 0;           // Move beam to the top and keep it there
            frame = 1 - frame;
            vSyncAccepted = false;
        }
    }
}

void GateArray::generateInterrupts() {

    if (crtc.vSync && !vSync_d) {
        hCounter = 0x2;
    }
    // In HSYNC falling edges, intCounter and hCounter are increased.
    // If intCounter reaches 52, an INT is generated and the counter
    // is reset.
    if (!crtc.hSync && hSync_d) {
        uint_fast32_t oldCounter = intCounter;

        if (++intCounter == 52) {
            intCounter = 0;
        }

        if (hCounter < 0x1c) {
            if (++hCounter == 0x4) {
                intCounter = 0;
            }
        }

        if (intCounter == 0 && (oldCounter & 0x20)) {
            z80_c &= ~SIGNAL_INT_;
        }
    }
}

void GateArray::paint() {

    if (!blanking) {

        uint32_t col = colours[inksel ? pens[pixelTable[actMode][colour]] : border];

        switch (scanlines) {
            case 1:     // Interlaced odd and even fields
                pixelsX2[(X_SIZE * (yPos + frame)) + xPos] = col;
                break;

            case 2:     // Averaged odd and even fields
                {
                    uint32_t *ptr = pixelsX2 + (2 * (yPos * X_SIZE + xPos));
                    ptr[frame] = col;
                    pixelsX1[yPos * X_SIZE + xPos] = average(ptr);
                }
                break;
            case 3:     // Only odd field, with scanlines
                pixelsX2[(X_SIZE * yPos) + xPos] = col;
#if SPECIDE_BYTE_ORDER == 1
                pixelsX2[(X_SIZE * (yPos + 1)) + xPos] =
                    (((col & 0xFEFEFE00) >> 1) + ((col & 0xFCFCFC00) >> 2) + ((col & 0xF8F8F800) >> 3)) | 0x000000FF;
#else
                pixelsX2[(X_SIZE * (yPos + 1)) + xPos] =
                    (((col & 0x00FEFEFE) >> 1) + ((col & 0x00FCFCFC) >> 2) + ((col & 0x00F8F8F8) >> 3)) | 0xFF000000;
#endif
                break;

            default:    // Odd field only, no scanlines
                pixelsX1[yPos * X_SIZE + xPos] = col;
                break;
        }

        switch (modeTable[actMode][counter & 0x7]) {
            case MOVE:
                colour = (colour << 1) & 0xFF; break;
            case LOAD:
                colour = videoByte; inksel = dispen; break;
            default: break;
        }
    } else {
        switch (scanlines) {
            case 1:
                pixelsX2[((yPos + frame) * X_SIZE) + xPos] = colours[20];
                break;
            case 2:
                pixelsX1[(yPos * X_SIZE) + xPos] = colours[20];
                break;
            case 3:
                pixelsX2[(yPos * X_SIZE) + xPos] = colours[20];
                pixelsX2[((yPos + 1) * X_SIZE) + xPos] = colours[20];
                break;
            default:
                pixelsX1[(yPos * X_SIZE) + xPos] = colours[20];
                break;
        }
    }
    xPos += xInc;
}

void GateArray::reset() {

    lowerRom = true;
    upperRom = true;
    actMode = 0;
    border = 0x10;
    z80_c = ~SIGNAL_RESET_;
    counter = 0;
    hCounter = 0;
    intCounter = 0;
}
// vim: et:sw=4:ts=4
