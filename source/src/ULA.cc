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
#include "KeyBinding.h"

#include <cmath>

using namespace std;

uint_fast32_t ULA::voltages[4][4] = {
    {391, 728, 3653, 3790}, // ULA 5C (Issue 2)
    {342, 652, 3591, 3753}, // ULA 6C (Issue 3)
    {342, 652, 3591, 3753}, // ULA 7C (128K)
    {342, 652, 3591, 3753}  // GA40085 (+2A/+3)
};

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
    false, false, false, true, false, false, false, true
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

    if (pixel == videoStart) {
        video = (scan < vBorderStart);
    } else if (pixel == hBorderStart) {
        border = true;
    } else if (pixel == videoEnd) {
        video = false;
    } else if (pixel == hBlankStart) {
        blanking = true;
        ++scan;

        if (scan == vSyncEnd) {
            vSync = true;
            frame = 1 - frame;
            yPos = 0;
        } else if (scan == vBlankEnd) { // As long as vSyncEnd != vBlankEnd
            flash += 0x08;
        } else if (scan == maxScan) {
            scan = 0;
        }
    } else if (pixel == hSyncEnd) {
        xPos = 0;
    } else if (pixel == hBlankEnd) {
        blanking = (scan >= vBlankStart) && (scan <= vBlankEnd);
        if (blanking == false) {
            yPos += yInc;
        }
    } else if (pixel == maxPixel) {
        pixel = 0;
        border = (scan >= vBorderStart);
        ulaReset = false;
        dataAddr = ((scan & 0x38) << 2)          // 00076210 54376540
            | ((scan & 0x07) << 8)
            | ((scan & 0xC0) << 5);

        attrAddr = ((scan & 0xF8) << 2)          // 00000076 54376540
            | 0x1800;
    }
}

void ULA::generateInterrupt() {

    if (scan == vSyncStart) {
        if (pixel == interruptStart && scan == vSyncStart) {
            scanKeys();
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
    bool memContention = contendedBank && z80Clk;
    bool memContentionOff = !(z80_c & SIGNAL_MREQ_);

    // I/O Contention
    // We detect I/O contended states whenever the address is even (A0 = 0)
    // and IORQ is low.
    // We only delay T2H. We do this by checking a delayed version of IORQ.
    bool ioUlaPort = !(z80_a & 0x0001);
    bool iorqLow = !(z80_c & SIGNAL_IORQ_);                     // T2 TW T3
    bool iorqLow_d = !(z80_c_2 & SIGNAL_IORQ_);                 // TW T3 T1
    bool ioContention = ioUlaPort && iorqLow && z80Clk;         // T2 TW T3
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

    // First attempt at MIC/EAR feedback loop.
    // Let's keep this here for the moment.
    static uint_fast32_t capacitor = 0;
    static uint_fast32_t chargeDelay = 0;

    // 128K has MIC and EAR connected together, so the capacitor loop applies.
    if (ulaVersion < 3) {
        uint_fast32_t v = voltage[soundBits];

        if (v > 3000) {
            ear = v;
            if (chargeDelay < 86400) {
                capacitor = 368 + (chargeDelay / 16);
                ++chargeDelay;
            }
        } else {
            chargeDelay = 0;
            if (!capacitor) {
                ear = v;
            } else {
                --capacitor;
            }
        }
    }

    // Tape input forces values on EAR pin.
    if ((tapeIn & 0xC0) == 0x80) ear = 342;
    if ((tapeIn & 0xC0) == 0xC0) ear = 3790;
}

void ULA::scanKeys() {

    if (!pollKeys) return;

    for (size_t ii = 0; ii < 8; ++ii) {
        keys[ii] = 0xFF;
    }

    for (size_t ii = 0; ii < sizeof(singleKeys) / sizeof(KeyBinding); ++ii) {
        if (Keyboard::isKeyPressed(singleKeys[ii].keyName)) {
            keys[singleKeys[ii].row] &= ~singleKeys[ii].key;
        }
    }

    for (size_t ii = 0; ii < sizeof(capsKeys) / sizeof(KeyBinding); ++ii) {
        if (Keyboard::isKeyPressed(capsKeys[ii].keyName)) {
            keys[capsKeys[ii].row] &= ~capsKeys[ii].key;
            keys[7] &= 0xFE;    // Press Caps Shift
        }
    }

    for (size_t ii = 0; ii < sizeof(symbolKeys) / sizeof(KeyBinding); ++ii) {
        if (Keyboard::isKeyPressed(symbolKeys[ii].keyName)) {
            keys[symbolKeys[ii].row] &= ~symbolKeys[ii].key;
            keys[0] &= 0xFD;    // Press Symbol Shift
        }
    }

    for (size_t ii = 0; ii < sizeof(spectrumKeyJoystick) / sizeof(JoystickKeyBinding); ++ii) {
        if (sinclairData & (1 << ii)) {
            keys[spectrumKeyJoystick[ii].row] &= ~spectrumKeyJoystick[ii].key;
        }
    }

    // Activate Caps Lock (Caps Shift + 2) when both Shifts are presseda.
    if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::RShift)) {
        keys[7] &= 0xFE;    // Press Caps Shift
        keys[4] &= 0xFD;    // Press 2
    }
}

uint_fast8_t ULA::ioRead() {

    uint_fast8_t byte = inMask;
    byte |= (ear < 700) ? 0x00 : 0x40;

    for (uint_fast8_t ii = 0; ii < 8; ++ii) {
        if (!(z80_a & (0x8000 >> ii))) {
            byte &= keys[ii];
        }
    }

    return byte;
}

void ULA::ioWrite(uint_fast8_t byte) {

    soundBits = (byte & 0x18) >> 3;

    borderAttr = byte & 0x07;
    if (ulaVersion == 5) {
        if (!video) {
            colour[1] = colourTable[0x80 | borderAttr];
        }
    }
}

void ULA::beeper() {

    // Smooth the signal directly from the ULA.
    if (playSound) {
        filter[index] = (soundBits & 0x02) ? SOUND_VOLUME : 0;
        if (tapeSound) {
            // In Spectrum 48K, ULA.b3 only causes sound if ULA.b4 is set.
            // In Spectrum 128K, ULA.b3 causes sound on its own.
            filter[index] +=
                + (((soundBits & micMask) == micMask) ? SAVE_VOLUME : 0)
                + ((tapeIn & 0x40) ? LOAD_VOLUME : 0);
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
        z80Clk = !z80Clk;
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
    z80Clk = false;
    cpuClock = true;
    video = true;
    border = false;
    z80_c_2 = z80_c_1 = 0xFFFF;
}

void ULA::reset() {

    ulaReset = true;
}

void ULA::setUlaVersion(uint_fast8_t version) {

    ulaVersion = version;
    paintPixel = 0x04;
    micMask = 0x03;

    videoStart = 0x008;
    videoEnd = 0x108;
    hBlankStart = 0x140;
    hBlankEnd = 0x19F;

    hBorderStart = 0x100;

    vBorderStart = 0x0C0;
    vBlankStart = 0x0F8;
    vBlankEnd = 0x0FF;
    vSyncStart = 0x0F8;
    vSyncEnd = 0x0FC;

    generateVideoData = &ULA::generateVideoDataUla;

    switch (ulaVersion) {
        case 0: // 48K, Issue 2
            hSyncEnd = 0x170;
            maxPixel = 0x1C0;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
        case 1: // 48K, Issue 3
            hSyncEnd = 0x178;
            maxPixel = 0x1C0;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x138;
            break;
        case 2: // 128K
            hSyncEnd = 0x178;
            maxPixel = 0x1C8;
            interruptStart = 0x004;
            interruptEnd = 0x04A;
            maxScan = 0x137;
            micMask = 0x01;
            break;
        case 3: // +2 (128K with late timings)
            hSyncEnd = 0x178;
            maxPixel = 0x1C8;
            interruptStart = 0x002;
            interruptEnd = 0x04A;
            maxScan = 0x137;
            micMask = 0x01;
            break;
        case 4: // +2A, +3
            paintPixel = 0x06;
            hBorderStart = 0x104;
            hSyncEnd = 0x178;
            maxPixel = 0x1C8;
            interruptStart = 0x000;
            interruptEnd = 0x040;
            maxScan = 0x137;
            cpuClock = true;
            micMask = 0x01;
            snow = false;
            idle = true;
            generateVideoData = &ULA::generateVideoDataGa;
            break;
        case 5: // Pentagon
            paintPixel = 0x02;
            hSyncEnd = 0x158;
            hBlankStart = 0x138;
            hBlankEnd = 0x198;
            maxPixel = 0x1C0;
            vBlankStart = 0x0F0;
            vBlankEnd = 0x100;
            vSyncStart = 0x0F0;
            maxScan = 0x140;
            cpuClock = true;
            interruptStart = 0x140;
            interruptEnd = 0x180;
            snow = false;
            idle = true;
            mem = false;
            generateVideoData = &ULA::generateVideoDataPentagon;
            break;
        default:
            hBorderStart = 0x101;
            hSyncEnd = 0x178;
            maxPixel = 0x1C0;
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
        false, false, false, true, false, false, false, true
    };

    // Cycle states for Gate Array based Spectrums (+2A/+3)
    bool delayGa[16] = {
        true, true, false, false, true, true, true, true,
        true, true, true, true, true, true, true, true
    };
    bool memGa[16] = {
        true, true, true, true, true, true, true, true,
        false, false, false, true, false, false, false, true
    };

    for (uint_fast8_t ii = 0; ii < 16; ++ii) {
        if (ulaVersion == 4) {
            // +2A/+3 has no floating bus.
            // idleTable is not necessary for +2A/+3; idle = true always.
            delayTable[ii] = delayGa[ii];
            memTable[ii] = memGa[ii];
        } else if (ulaVersion == 5) {
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

    for (uint_fast32_t ii = 0; ii < 4; ++ii) {
        voltage[ii] = voltages[ulaVersion][ii];
    }
}
// vim: et:sw=4:ts=4:
