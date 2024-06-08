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

#include "ULA.h"

#include <cassert>
#include <cmath>

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

uint32_t ULA::colourTable[0x100];
uint8_t ULA::averageTable[0x100][0x100];
uint32_t ULA::pixelsX1[X_SIZE * Y_SIZE / 2];
uint32_t ULA::pixelsX2[X_SIZE * Y_SIZE];

ULA::ULA() :
    keys{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    keyData{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    c(0xFFFF) {

        for (uint_fast32_t i = 0; i < 0x100; ++i) {
            // Generate colour table.
            // dhpppiii
#if SPECIDE_BYTE_ORDER == 1
            uint32_t colour = (i & 0x80) ?
                ((i & 0x02) << 23) | ((i & 0x04) << 14) | ((i & 0x01) << 8) :
                ((i & 0x10) << 20) | ((i & 0x20) << 11) | ((i & 0x08) << 5);
#else
            uint32_t colour = (i & 0x80) ?
                ((i & 0x02) >> 1) | ((i & 0x04) << 6) | ((i & 0x01) << 16) :
                ((i & 0x10) >> 4) | ((i & 0x20) << 3) | ((i & 0x08) << 13);
#endif
            colour *= (i & 0x40) ? 0xFF : 0xC0;
#if SPECIDE_BYTE_ORDER == 1
            colour |= 0xFF;
#else
            colour |= (0xFF << 24);
#endif
            colourTable[i] = colour;
        }

        for (size_t i = 0x00; i < 0x100; ++i) {
            for (size_t j = 0x00; j < 0x100; ++j) {
                averageTable[i][j] = static_cast<uint8_t>(sqrt(((i * i) + (j * j)) / 2));
            }
        }
    }

/**
 * Average two colours in consecutive positions.
 */
uint32_t ULA::average(uint32_t *ptr) {

    uint8_t *pSrc = reinterpret_cast<uint8_t*>(ptr);
#if SPECIDE_BYTE_ORDER == 1
    return (0xFF
            | averageTable[pSrc[1]][pSrc[5]] << 8
            | averageTable[pSrc[2]][pSrc[6]] << 16
            | averageTable[pSrc[3]][pSrc[7]] << 24);
#else
    return (0xFF << 24
            | averageTable[pSrc[2]][pSrc[6]] << 16
            | averageTable[pSrc[1]][pSrc[5]] << 8
            | averageTable[pSrc[0]][pSrc[4]]);
#endif
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
        if (pixel == interruptStart && scan == vSyncStart) {
            // Keyboard is updated here to avoid split double keys.
            for (size_t ii = 0; ii < 8; ++ii) keyData[ii] = keys[ii];
            z80_c &= ~SIGNAL_INT_;
        } else if (pixel == interruptEnd && scan == vSyncStart) {
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
        colour[0] = colourTable[(0x00 ^ (attr & flash & 0x80)) | (attr & 0x7F)];
        colour[1] = colourTable[(0x80 ^ (attr & flash & 0x80)) | (attr & 0x7F)];
    }
}

void ULA::paint() {

    if (!blanking) {
        ++xPos;

        uint32_t col = colour[(data >> 7)];
        data <<= 1;

        switch (scanlines) {
            case 1:     // Scanlines
                pixelsX2[(X_SIZE * (yPos + frame)) + xPos] = col;
                break;

            case 2:     // Averaged scanlines
                {
                    uint32_t *ptr = pixelsX2 + (2 * (yPos * X_SIZE + xPos));
                    ptr[frame] = col;
                    pixelsX1[yPos * X_SIZE + xPos] = average(ptr);
                }
                break;

            default:    // No scanlines
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
            colour[1] = colourTable[0x80 | borderAttr];
        }
    }
}

void ULA::beeper() {

    // Smooth the signal directly from the ULA.
    if (playSound) {
        filter[index] = (soundBits & 0x02) ? ULA_BEEP_VOLUME : 0;
        if (tapeSound) {
            // In Spectrum 48K, ULA.b3 only causes sound if ULA.b4 is set.
            // In Spectrum 128K, ULA.b3 causes sound on its own.
            filter[index] +=
                + (((soundBits & micMask) == micMask) ? ULA_SAVE_VOLUME : 0)
                + (tapeLevel ? ULA_LOAD_VOLUME : 0);
        }
    } else {
        filter[index] = 0x00;
    }

    index = (index + 1) % FILTER_BZZ_SIZE;
}

int ULA::sample() {

    int sound = 0;
    for (size_t i = 0; i < FILTER_BZZ_SIZE; ++i) {
        sound += filter[i];
    }
    sound /= FILTER_BZZ_SIZE;
    return sound;
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
