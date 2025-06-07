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

#include <cassert>

#include "ULA.h"
#include "Utils.h"

using namespace std;

bool ULA::delayTable[16] = {
    false, false, false, true, true, true, true, true,
    true, true, true, true, true, true, true, false
};

bool ULA::idleTable[16] = {
    true, true, true, true, true, true, true, true,
    false, false, false, false, false, false, false, false
};

bool ULA::memTable[16] = {
    true, true, true, true, true, true, true, true,
    false, true, false, true, false, true, false, true
};

uint_fast32_t ULA::snowTable[16] = {
    NONE, NONE, NONE, NONE, SNOW, NONE, NONE, NONE,
    DUPL, NONE, NONE, NONE, ENDS, NONE, NONE, ENDD
};

#if SPECIDE_BYTE_ORDER == 1
uint32_t ULA::palette[ULA_NUM_PALETTES][0x10] = {
    // Colour TV set
    {
        0x000000FF, 0x0000C0FF, 0xC00000FF, 0xC000C0FF, 0x00C000FF, 0x00C0C0FF, 0xC0C000FF, 0xC0C0C0FF,
        0x000000FF, 0x0000FFFF, 0xFF0000FF, 0xFF00FFFF, 0x00FF00FF, 0x00FFFFFF, 0xFFFF00FF, 0xFFFFFFFF
    },
    // Black & White TV set
    {
        0xFF000000, 0xFF0000D8, 0xFF00D800, 0xFF00D8D8, 0xFFD80000, 0xFFD800D8, 0xFFD8D800, 0xFFD8D8D8,
        0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF
    },
    // Green phosphor monitor
    {
        0xFF000000, 0xFF0000D8, 0xFF00D800, 0xFF00D8D8, 0xFFD80000, 0xFFD800D8, 0xFFD8D800, 0xFFD8D8D8,
        0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF
    },
    // Amber phosphor monitor
    {
        0xFF000000, 0xFF0000D8, 0xFF00D800, 0xFF00D8D8, 0xFFD80000, 0xFFD800D8, 0xFFD8D800, 0xFFD8D8D8,
        0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF
    },
    // User defined palette
    {
        0xFF000000, 0xFF0000D8, 0xFF00D800, 0xFF00D8D8, 0xFFD80000, 0xFFD800D8, 0xFFD8D800, 0xFFD8D8D8,
        0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF
    }
};
#else
uint32_t ULA::palette[ULA_NUM_PALETTES][0x10] = {
    // Colour TV set
    {
        0xFF000000, 0xFFC00000, 0xFF0000C0, 0xFFC000C0, 0xFF00C000, 0xFFC0C000, 0xFF00C0C0, 0xFFC0C0C0,
        0xFF000000, 0xFFFF0000, 0xFF0000FF, 0xFFFF00FF, 0xFF00FF00, 0xFFFFFF00, 0xFF00FFFF, 0xFFFFFFFF
    },
    // Black & White TV set
    {
        0xFF000000, 0xFF161616, 0xFF393939, 0xFF4F4F4F, 0xFF717171, 0xFF878787, 0xFFAAAAAA, 0xFFC0C0C0,
        0xFF000000, 0xFF1D1D1D, 0xFF4C4C4C, 0xFF696969, 0xFF969696, 0xFFB3B3B3, 0xFFE2E2E2, 0xFFFFFFFF
    },
    // Green phosphor monitor
    {
        0xFF000000, 0xFF001600, 0xFF003900, 0xFF004F00, 0xFF007100, 0xFF008700, 0xFF00AA00, 0xFF00C000,
        0xFF000000, 0xFF001D00, 0xFF004C00, 0xFF006900, 0xFF009600, 0xFF00B300, 0xFF00E200, 0xFF00FF00
    },
    // Amber phosphor monitor
    {
        0xFF000000, 0xFF001016, 0xFF002B39, 0xFF003B4F, 0xFF005571, 0xFF006587, 0xFF0080AA, 0xFF0090C0,
        0xFF000000, 0xFF00161D, 0xFF00394C, 0xFF004F69, 0xFF007096, 0xFF0087B3, 0xFF00AAE2, 0xFF00C0FF
    },
    // User defined palette
    {
        0xFF000000, 0xFFD80000, 0xFF0000D8, 0xFFD800D8, 0xFF00D800, 0xFFD8D800, 0xFF00D8D8, 0xFFD8D8D8,
        0xFF000000, 0xFFFF0000, 0xFF0000FF, 0xFFFF00FF, 0xFF00FF00, 0xFFFFFF00, 0xFF00FFFF, 0xFFFFFFFF
    }
};
#endif

uint32_t ULA::colourTable[0x10];
uint32_t ULA::colour[2];
uint32_t ULA::pixelsX1[X_SIZE * Y_SIZE / 2];
uint32_t ULA::pixelsX2[X_SIZE * Y_SIZE];

ULA::ULA() :
    keys{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    keyData{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    c(0xFFFF) {}

void ULA::setPalette(uint32_t type) {

    if (type < ULA_NUM_PALETTES) {
        for (size_t ii = 0; ii < 0x10; ++ii) {
            colourTable[ii] = palette[type][ii];
        }
    }

    fillAverageTable();
}

void ULA::generateVideoControlSignals() {

    if (pixel == checkPointValues[ulaVersion][checkPoint]) {
        // I've simplified the if-else if-else tree using this block because
        // these stages always happen in the same order.
        switch (checkPoint) {
            case 0:     // On VideoStart
                video = (scan < vBorderStart);
                break;
            case 1:     // On HBorderStart
                border = true;
                break;
            case 2:     // On VideoEnd
                video = false;
                break;
            case 3:     // On HBlankStart
                blanking = true;
                ++scan;
                if (scan == vSyncEnd) {
                    vSync = true;
                    frame = 1 - frame;
                    yPos = 0;
                } else if (scan == vBlankEnd) {
                    flash += 0x8;
                } else if (scan == maxScan) {
                    scan = 0;
                    // Avoid problems caused by changing key data during the interrupt.
                    for (size_t ii = 0; ii < 8; ++ii) keyData[ii] = keys[ii];
                }
                break;
            case 4:     // On HBlankEnd
                xPos = 0;
                blanking = (scan >= vBlankStart) && (scan <= vBlankEnd);
                if (!blanking) {
                    yPos += yInc;
                }
                break;
            case 5:     // On MaxPixel
                pixel = 0;
                border = (scan >= vBorderStart);
                ulaReset = false;
                dataAddr = ((scan & 0x38) << 2) | ((scan & 0x07) << 8) | ((scan & 0xC0) << 5);  // ...76210 543xxxxx
                attrAddr = ((scan & 0xF8) << 2) | 0x1800;                                       // ...HHL76 543xxxxx
                break;
            default: assert(false); break;  // Should not happen
        }

        checkPoint = (checkPoint + 1) % NUM_CHECKPOINTS;
    }
}

void ULA::generateInterrupt() {

    if (scan == vSyncStart) {
        if (pixel == interruptStart) {
            z80_c &= ~SIGNAL_INT_;
        } else if (pixel == interruptEnd) {
            z80_c |= SIGNAL_INT_;
        }
    }
}

void ULA::generateVideoDataUla() {

    // Check for contended memory or I/O accesses.
    idle = idleTable[pixel & 0x0F];
    mem = memTable[pixel & 0x0F];
    snow = snowTable[pixel & 0x0F];

    // Memory Contention
    // We detect memory contended states whenever the address is in the
    // contention range (0x4000-0x7FFF).
    // We only delay T1H until the ULA has finished reading. The rest of
    // states are not contended. We do this by checking MREQ is low.
    // We contend T-States, which means we only consider high clock phase.
    bool memContention = contendedBank && z80Clock;
    bool memContentionOff = !(z80_c & SIGNAL_MREQ_);

    // I/O Contention
    // We detect I/O contended states whenever the address is even (A0 = 0)
    // and IORQ is low.
    // We only delay T2H. We do this by checking a delayed version of IORQ.
    bool ioUlaPort = !(z80_a & 0x0001);
    bool iorqLow = !(z80_c & SIGNAL_IORQ_);                     // T2 TW T3
    bool iorqLow_d = !(z80_c_2 & SIGNAL_IORQ_);                 // TW T3 T1
    bool ioContention = ioUlaPort && iorqLow && z80Clock;       // T2 TW T3
    bool ioContentionOff = ioUlaPort && iorqLow_d;              // TW T3 NN

    // Now, we have two different contention schemes in accesses
    // 1. We have pure memory accesses. Here, the contention will happen if
    //    the address is in 0x4000-0x7FFF range or 0xC000-0xFFFF for contended
    //    RAM pages. The contention is disabled when MREQ goes low. The first
    //    term handles this contention type.
    // 2. We have I/O ULA accesses when the port address is not like a contended
    //    memory address. In this case, the contention is disabled after IORQ
    //    is low for one cycle.
    // 3. We have I/O ULA accesses when the port address is like a contended memory
    //    address. Here we have the memory contention part before the I/O
    //    contention happens, then we have the I/O contention, until the I/O
    //    contention is lifted. The second term handles both cases.
    // 4. Finally, we have other I/O accesses where the I/O address looks
    //    like a contended memory address. In this case, no I/O
    //    contention happens, but MREQ never goes low, so every cycle is
    //    contended.
    bool contention = (memContention && !ioContention && !memContentionOff)
        || (ioContention && !ioContentionOff);

    // Resolve contention and generate CPU clock.
    cpuClock = !(contention && delayTable[pixel & 0x0F]);

    // Read from memory.
    switch (pixel & 0x0F) {
        case 0x08: a = dataAddr++; break;
        case 0x09: dataReg = d; break;
        case 0x0A: a = attrAddr++; break;
        case 0x0B: attrReg = d; break;
        case 0x0C: a = dataAddr++; break;
        case 0x0D: dataReg = d; break;
        case 0x0E: a = attrAddr++; break;
        case 0x0F: attrReg = d; break;
        default: break;
    }
}

void ULA::generateVideoDataGa() {

    // Check for contended memory or I/O accesses.
    mem = memTable[pixel & 0x0F];

    // Memory Contention
    // +2A/+3 only apply contention if MREQ is low.
    bool contention = contendedBank && !(z80_c & SIGNAL_MREQ_);

    if (contention && delayTable[pixel & 0x0F]) {
        z80_c &= ~SIGNAL_WAIT_;
    } else {
        z80_c |= SIGNAL_WAIT_;
    }

    // Read from memory.
    switch (pixel & 0x0F) {
        case 0x01: attrReg = latch; break;
        case 0x08: a = dataAddr++; break;
        case 0x09: latch = d; break;
        case 0x0A: a = attrAddr++; break;
        case 0x0B: dataReg = latch; latch = d; break;
        case 0x0C: a = dataAddr++; break;
        case 0x0D: attrReg = latch; latch = d; break;
        case 0x0E: a = attrAddr++; break;
        case 0x0F: dataReg = latch; latch = d; break;
        default: break;
    }
}

void ULA::generateVideoDataPentagon() {

    // Read from memory.
    switch (pixel & 0x0F) {
        case 0x05: a = dataAddr++; break;
        case 0x06: dataReg = d; a = attrAddr++; break;
        case 0x07: attrReg = d; break;

        case 0x0d: a = dataAddr++; break;
        case 0x0e: dataReg = d; a = attrAddr++; break;
        case 0x0f: attrReg = d; break;

        default: break;
    }
}

void ULA::updateAttributes() {

    if ((pixel & 0x07) == paintPixel) {
        data = video ? dataReg : 0xFF;
        attr = video ? attrReg : borderAttr;

        size_t indexBackground = (attr & flash & 0x80) >> 7;
        size_t indexForeground = 1 - indexBackground;
        colour[indexBackground] = colourTable[((attr & 0x78) >> 3)];
        colour[indexForeground] = colourTable[((attr & 0x40) >> 3) | (attr & 0x07)];
    }
}

void ULA::paint() {

    if (!blanking) {
        ++xPos;

        uint32_t col = colour[(data >> 7)];
        data <<= 1;

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
    }
}

void ULA::tapeEarMic() {

    // These operations are too costly to do them every cycle.
    static uint_fast32_t count = 0;
    if (ulaVersion < ULA_PLUS2 && !tapePlaying && !(++count & 0x3F)) {
        vInc *= 0.934375;
        vEar = vEnd - vInc;
    }
}

void ULA::setEarLevel(bool level, bool playing) {

    vEar = level ? 1.500 : 0.000;
    vInc = vEnd - vEar;
    tapeLevel = level;
    tapePlaying = playing;
}

uint_fast8_t ULA::ioRead() {

    uint_fast8_t byte = inMask;
    byte |= (vEar > 0.700) ? 0x40 : 0x00;

    for (uint_fast8_t ii = 0; ii < 8; ++ii) {
        if (!(z80_a & (0x8000 >> ii))) {
            byte &= keyData[ii];
        }
    }

    return byte;
}

void ULA::ioWrite(uint_fast8_t byte) {

    soundBits = (byte & 0x18) >> 3;
    borderAttr = byte & 0x07;

    if (ulaVersion < ULA_PLUS2) {
        if (!tapePlaying) {
            vEnd = voltages[ulaVersion][soundBits];
            vInc = vEnd - vEar;
        }
    } else if (ulaVersion == ULA_PENTAGON) {
        if (!video) {
            colour[1] = colourTable[borderAttr & 0x7];
        }
    }
}

void ULA::beeper() {

    // Smooth the signal directly from the ULA.
    uint_fast16_t level = 0;
    if (playSound) {
        level += (soundBits & 0x02) ? ULA_BEEP_VOLUME : 0;
        if (tapeSound) {
            // In Spectrum 48K, ULA.b3 only causes sound if ULA.b4 is set.
            // In Spectrum 128K, ULA.b3 causes sound on its own.
            level += (((soundBits & micMask) == micMask) ? ULA_SAVE_VOLUME : 0)
                + (tapeLevel ? ULA_LOAD_VOLUME : 0);
        }
    }

    filter.add(level);
}

int ULA::sample() {

    return filter.get();
}

void ULA::clock() {

    generateVideoControlSignals();
    generateInterrupt();

    if (!border) {
        (this->*generateVideoData)();
    }

    tapeEarMic();

    // Contention affects to Z80 phase change and to when the I/O operation
    // actually happens.
    if (cpuClock && (z80_c & SIGNAL_WAIT_)) {
        z80_c_2 = z80_c_1;
        z80_c_1 = z80_c;
        z80Clock = !z80Clock;
    }

    updateAttributes();
    paint();
    ++pixel;

    if (ulaReset) {
        start();
    }
}

void ULA::start() {

    pixel = 0;
    scan = 0;
    xPos = 0;
    yPos = 0;
    ulaReset = false;
    z80Clock = false;
    cpuClock = true;
    video = true;
    border = false;
    z80_c_2 = z80_c_1 = 0xFFFF;
    checkPoint = 0;
}

void ULA::reset() {

    ulaReset = true;
}

void ULA::setUlaVersion(uint_fast8_t version) {

    ulaVersion = version;
    paintPixel = 0x04;
    micMask = 0x03;

    vBorderStart = 0x0C0;
    vBlankStart = 0x0F8;
    vBlankEnd = 0x0FF;
    vSyncStart = 0x0F8;
    vSyncEnd = 0x0FC;

    generateVideoData = &ULA::generateVideoDataUla;

    switch (ulaVersion) {
        case ULA_48KISS2:   // Spectrum 48K (Ferranti 5C/6C)
        case ULA_48KISS3:   // fall-through
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
        case ULA_128K:      // Spectrum 128K (Ferranti 7K)
            interruptStart = 0x004;
            interruptEnd = 0x04A;
            maxScan = 0x137;
            micMask = 0x01;
            break;
        case ULA_PLUS2:     // Spectrum +2 (Amstrad 40056)
            interruptStart = 0x002;
            interruptEnd = 0x04A;
            maxScan = 0x137;
            micMask = 0x01;
            break;
        case ULA_PLUS3:     // Spectrum +2A/+2B/+3 (Amstrad 40077)
            paintPixel = 0x06;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x137;
            cpuClock = true;
            micMask = 0x01;
            snow = NONE;
            idle = true;
            generateVideoData = &ULA::generateVideoDataGa;
            break;
        case ULA_PENTAGON:  // Pentagon video circuitry
            paintPixel = 0x02;
            vBlankStart = 0x0F0;
            vBlankEnd = 0x100;
            vSyncStart = 0x0F0;
            maxScan = 0x140;
            cpuClock = true;
            interruptStart = 0x140;
            interruptEnd = 0x180;
            snow = NONE;
            idle = true;
            mem = false;
            generateVideoData = &ULA::generateVideoDataPentagon;
            break;
        default:
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
    }

    // Delay interrupt by one ULA clock cycle. In real hardware, this would
    // happen normally, because Z80 is running in parallel with the ULA,
    // but here the ULA is emulated first, and then the Z80.
    ++interruptStart;
    ++interruptEnd;

    // Cycle states for ULA-based Spectrums (48K/128K/+2)
    bool delayUla[16] = {
        false, false, false, true, true, true, true, true,
        true, true, true, true, true, true, true, false
    };
    bool memUla[16] = {
        true, true, true, true, true, true, true, true,
        false, true, false, true, false, true, false, true
    };

    // Cycle states for Gate Array based Spectrums (+2A/+3)
    bool delayGa[16] = {
        true, true, false, false, true, true, true, true,
        true, true, true, true, true, true, true, true
    };
    bool memGa[16] = {
        true, true, true, true, true, true, true, true,
        false, true, false, true, false, true, false, true
    };

    for (uint_fast8_t ii = 0; ii < 16; ++ii) {
        if (ulaVersion == ULA_PLUS3) {
            // +2A/+3 has no floating bus.
            // idleTable is not necessary for +2A/+3; idle = true always.
            delayTable[ii] = delayGa[ii];
            memTable[ii] = memGa[ii];
        } else if (ulaVersion == ULA_PENTAGON) {
            // Pentagon has neither floating bus, nor contention.
            // idleTable is not necessary for Pentagon; idle = true always,
            // and delay is irrelevant.
            delayTable[ii] = false;
            memTable[ii] = false;
        } else {
            delayTable[ii] = delayUla[ii];
            memTable[ii] = memUla[ii];
        }
    }
}

// vim: et:sw=4:ts=4:
