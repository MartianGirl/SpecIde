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

#include "CPC.h"
#include "KeyBinding.h"
#include "SpecIde.h"

#include <cstdlib>
#include <ctime>

int constexpr SAVE_VOLUME = 0x03FF;
int constexpr LOAD_VOLUME = 0x01FF;

CPC::CPC() {

    setPage(0, 0);
    setPage(1, 1);
    setPage(2, 2);
    setPage(3, 3);

    for (size_t ii = 0; ii < 10; ++ii) {
        keys[ii] = 0xFF;
    }

    channel.open(2, SAMPLE_RATE);
}

void CPC::loadRoms(RomVariant model) {

    string romName;
    switch (model) {
        case RomVariant::ROM_CPC464:
        romName = "cpc464.rom";
        break;

        case RomVariant::ROM_CPC664:
        romName = "cpc664.rom";
        break;

        case RomVariant::ROM_CPC6128:
        romName = "cpc6128.rom";
        break;

        default:
        romName = "cpc464.rom";
        break;
    }

    vector<string> romDirs = getRomDirs();
    size_t j = 0;
    bool fail = true;
    do {
        string romPath = romDirs[j] + romName;
        cout << "Trying ROM: " << romPath << "... ";

        ifstream ifs(romPath, ifstream::binary);
        // Logical || is a short-circuit operator.
        fail = ifs.fail() || ifs.read(reinterpret_cast<char*>(rom), 0x8000).fail();
        cout << string(fail ? "FAIL" : "OK") << endl;
        ++j;
    } while (fail && j < romDirs.size());
}

void CPC::loadExpansionRoms() {

    vector<string> romDirs = getRomDirs();
    char buffer[0x4000];

    for (size_t ii = 0; ii < 0x100; ++ii) {
        extReady[ii] = false;
    }

    for (map<uint8_t, ExpansionRom>::iterator it = ext.begin(); it != ext.end(); ++it) {
        size_t j = 0;
        bool fail = true;
        string romName = it->second.name;
        uint8_t romSlot = it->first;
        if (!romName.empty()) do {
            string romPath = romDirs[j] + romName;
            cout << "Trying ROM in Slot #"
                << static_cast<uint32_t>(romSlot) << ": " << romPath << "... ";
            ifstream ifs(romPath, ifstream::binary);
            fail = ifs.fail() || ifs.read(buffer, 0x4000).fail();
            it->second.data.assign(buffer, &buffer[0x4000]);
            extReady[romSlot] = !fail;
            cout << string(fail ? "FAIL" : "OK") << endl;
            ++j;
        } while (fail && j < romDirs.size());
    }
}

void CPC::set464() {

    cpc128K = false;
    cpcDisk = false;
    expBit = false;

    loadRoms(RomVariant::ROM_CPC464);
    loadExpansionRoms();
    reset();
}

void CPC::set664() {

    cpc128K = false;
    cpcDisk = true;
    expBit = false;
    fdc765.clockFrequency = 4;

    ext[0x07] = ExpansionRom("amsdos.rom");

    loadRoms(RomVariant::ROM_CPC664);
    loadExpansionRoms();
    reset();
}

void CPC::set6128() {

    cpc128K = true;
    cpcDisk = true;
    expBit = false;
    fdc765.clockFrequency = 4;

    ext[0x07] = ExpansionRom("amsdos.rom");

    loadRoms(RomVariant::ROM_CPC6128);
    loadExpansionRoms();
    reset();
}

void CPC::playSound(bool play) {

    static bool playing = false;
    if (!playing && play) {
        channel.play();
        playing = true;
    } else if (playing && !play) {
        channel.stop();
        playing = false;
    }
}

void CPC::run(bool frame) {

    cycles = 0;
    ga.sync = false;

    if (frame) {
        while(!ga.sync) {
            clock();
            generateSound();
            ++cycles;
        }
    } else {
        while (cycles++ < 16 * FRAME_TIME_CPC) {
            clock();
            generateSound();
        }
    }
}

void CPC::generateSound() {

    static double remaining = 0;

    // Generate sound. This maybe can be done using the same counter?
    if (!(--skipCycles)) {
        skipCycles = skip;
        remaining += tail;
        if (remaining >= 1.0) {
            skipCycles++;
            remaining -= 1.0;
        }
        sample();
    }
}

void CPC::clock() {

    bool m1_ = z80.c & SIGNAL_M1_;
    bool as_ = z80.c & SIGNAL_MREQ_;
    bool io_ = z80.c & SIGNAL_IORQ_;
    size_t memArea = z80.a >> 14;

    if (!io_) {
        // Gate Array. This is accessible by both I/O reads and I/O writes.
        // &7Fxx is Gate Array / RAM selection port.
        if ((z80.a & 0xC000) == 0x4000) {
            if ((z80.d & 0xC0) == 0xC0) {
                if (cpc128K && z80.wr) {
                    selectRam(z80.d);
                }
            } else if (m1_) {
                ga.write(z80.d);
            }
        }

        // ROM expansion selection.
        // &DFxx is ROM selection port.
        if (z80.wr && !(z80.a & 0x2000)) {
            romBank = z80.d;
            if (romBank && extReady[romBank]) {
                hiRom = &ext[romBank].data[0];
            } else {
                romBank = 0;
                hiRom = &rom[0x4000];
            }
        }

        // CRTC.
        // &BCxx, &BDxx, &BExx, &BFxx are CRTC ports.
        if ((z80.rd || z80.wr) && ((z80.a & 0x4000) == 0x0000)) {
            switch (z80.a & 0x0300) {
                case 0x0000:    // CRTC Register Select (WO) at &BC00.
                    ga.crtc.wrAddress(z80.d);
                    break;
                case 0x0100:    // CRTC Register Write (WO) at &BD00.
                    ga.crtc.wrRegister(z80.d);
                    break;
                case 0x0200:    // CRTC Status Register Read (type 1) at &BE00.
                    ga.crtc.rdStatus(ga.d);
                    break;
                case 0x0300:    // CRTC Register Read (RO) at &BF00.
                    ga.crtc.rdRegister(ga.d);
                    break;
                default:
                    break;
            }
        }

        // 8255 PPI.
        // &F4xx, &F5xx, &F6xx, &F7xx are 8255 ports.
        if ((z80.a & 0x0800) == 0x0000) {
            switch (z80.a & 0x0300) {
                case 0x0000:    // Port A: &F4xx
                    if (z80.rd) {
                        z80.d = ppi.readPortA();
                    } else if (z80.wr) {
                        ppi.writePortA(z80.d);
                    }
                    break;
                case 0x0100:    // Port B: &F5xx
                    if (z80.rd) {
                        ppi.portB = tapeLevel | 0x5E | (expBit ? 0x20 : 0x00) | (ga.crtc.vSync ? 0x1 : 0x0);
                        z80.d = ppi.readPortB();
                    } else if (z80.wr) {
                        ppi.writePortB(z80.d);
                    }
                    break;
                case 0x0200:    // Port C: &F6xx
                    if (z80.rd) {
                        z80.d = ppi.readPortC();
                    } else if (z80.wr) {
                        ppi.writePortC(z80.d);
                    }
                    break;
                case 0x0300:    // Control port: &F7xx
                    if (z80.wr) {
                        ppi.writeControlPort(z80.d);
                    }
                    break;
                default:
                    break;
            }

            if (!ppi.inputLoC) {
                psg.setPortA(keys[ppi.portC & 0x0F]);
            }

            if (!ppi.inputHiC) {
                relay = ppi.portC & 0x10;

                // Execute PSG command.
                switch (ppi.portC & 0xC0) {
                    case 0x40:
                        ppi.portA = ppi.inputA ? psg.read() : 0x00;
                        break;
                    case 0x80:
                        psg.write(ppi.portA);
                        break;
                    case 0xC0:
                        psg.addr(ppi.portA);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    if (io_ || ga.blockIorq()) {
        ga.d = ram[ga.crtc.byteAddress | ga.cClkOffset()];
    } else {
        ga.d = z80.d;
    }

    // First we clock the Gate Array. Further clocks will be generated here.
    ga.clock();

    if (ga.psgClock()) {
        psg.clock();
    }

    // The FDC chip is clocked at 4MHz, only rising edges.
    if (cpcDisk && ga.fdcClock()) {
        fdc765.clock();
    }

    // Z80 gets data from the ULA or memory, only when reading.
    // Z80 is clocked at 4MHz, but acts on both rising and falling edges.
    if (ga.cpuClock()) {
        z80.c = ga.z80_c;

        if (relay) {
            if (tapeSpeed < 686000) {
                ++tapeSpeed;
            }
        } else {
            if (tapeSpeed) {
                --tapeSpeed;
            }
        }

        if (tape.playing && tapeSpeed) {
            // 400000 @ 4MHz = 0.1s
            filter[index] = (tapeLevel && tapeSound) ? LOAD_VOLUME : 0;
            index = (index + 1) % FILTER_BZZ_SIZE;

            if (!tape.sample--) {
                uint_fast8_t level = tape.advance();
                tapeLevel = (tapeSpeed >= 343000) ? ((level & 0x40) << 1) : 0x00;
            }
        } else {
            tapeLevel = 0x00;
        }

        if (!io_) {
            // Peripherals
            if ((z80.a & 0x0400) == 0x0000) {

                // FDC
                if (cpcDisk && (z80.a & 0x0480) == 0x0000) {
                    switch (z80.a & 0x0101) {
                        case 0x0100:    // FDC main status register (&FB7E)
                            if (z80.rd) {
                                z80.d = fdc765.status();
                            }
                            break;
                        case 0x0101:    // FDC data register (&FB7F)
                            if (z80.wr) {
                                fdc765.write(z80.d);
                            } else if (z80.rd) {
                                z80.d = fdc765.read();
                            }
                            break;
                        case 0x0000:    // FDC motor (&FA7E)
                            if (z80.wr) {
                                fdc765.motor((z80.d & 0x01) == 0x01);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        } else if (!as_) {
            if (z80.rd) {
                switch (memArea) {
                    case 0:
                        z80.d = ga.lowerRom ? loRom[z80.a & 0x3FFF] : mem[0][z80.a & 0x3FFF];
                        break;
                    case 3:
                        z80.d = ga.upperRom ? hiRom[z80.a & 0x3FFF] : mem[3][z80.a & 0x3FFF];
                        break;
                    default:
                        z80.d = mem[memArea][z80.a & 0x3FFF];
                        break;
                }
            } else if (z80.wr) {
                mem[memArea][z80.a & 0x3FFF] = z80.d;
            }
        }

        z80.clock();
        ga.z80_c = z80.c;
    }
}

void CPC::reset() {

    ga.reset();
    ga.crtc.reset();
    selectRam(0);

    z80.reset();
    psgReset();
    fdc765.reset();
}

void CPC::psgReset() {

    psg.reset();
    psg.seed = 0xFFFF;
}

void CPC::psgPlaySound(bool play) {

    psg.playSound = play;
}

void CPC::psgChip(bool aychip) {

    psg.setVolumeLevels(aychip);
}

void CPC::sample() {

    int sound = 0;
    int l = 0;
    int r = 0;

    for (size_t ii = 0; ii < FILTER_BZZ_SIZE; ++ii) {
        sound += filter[ii];
    }
    sound /= FILTER_BZZ_SIZE;
    l = r = sound;

    psg.sample();

    switch (stereo) {
        case StereoMode::STEREO_ACB: // ACB
            l += psg.channelA;
            l += psg.channelC;
            r += psg.channelB;
            r += psg.channelC;
            break;

        case StereoMode::STEREO_ABC: // ABC
            l += psg.channelA;
            l += psg.channelB;
            r += psg.channelB;
            r += psg.channelC;
            break;

        default:    // mono, all channels go through both sides.
            l += psg.channelA;
            l += psg.channelB;
            l += psg.channelC;
            r += psg.channelA;
            r += psg.channelB;
            r += psg.channelC;
            break;
    }

    l = 2 * (l - 0x4000);
    r = 2 * (r - 0x4000);
    channel.push(l, r);
}

void CPC::selectRam(uint_fast8_t byte) {

    switch (byte & 0x7) {
        case 0: // Bank 0, first screen buffer (0-1-2-3)
            setPage(0, 0);
            setPage(1, 1);
            setPage(2, 2);
            setPage(3, 3);
            break;

        case 1: // Bank 0, second screen buffer (0-1-2-7)
            setPage(0, 0);
            setPage(1, 1);
            setPage(2, 2);
            setPage(3, 7);
            break;

        case 2: // Bank 1
            setPage(0, 4);
            setPage(1, 5);
            setPage(2, 6);
            setPage(3, 7);
            break;

        case 3: // Bank 0, screen 1 at $4000-$7fff, screen 2 at $c000-$ffff
            setPage(0, 0);
            setPage(1, 3);
            setPage(2, 2);
            setPage(3, 7);
            break;

        case 4: // fall-through
        case 5: // fall-through
        case 6: // fall-through
        case 7: // Bank 0, Bank 1 page N at $4000-$7fff
            setPage(0, 0);
            setPage(1, byte & 0x7);
            setPage(2, 2);
            setPage(3, 3);
            break;

        default:
            break;
    }
}

void CPC::setPage(uint_fast8_t page, uint_fast8_t bank) {

    size_t addr = bank * (1 << 14);
    mem[page] = &ram[addr];
}

void CPC::scanKeys() {

    if (!pollKeys) return;

    for (size_t ii = 0; ii < 16; ++ii) {
        keys[ii] = 0xFF;
    }

    for (size_t ii = 0; ii < sizeof(cpcKeys) / sizeof(KeyBinding); ++ii) {
        if (Keyboard::isKeyPressed(cpcKeys[ii].keyName)) {
            keys[cpcKeys[ii].row] &= ~cpcKeys[ii].key;
        }
    }

    for (size_t jj = 0; jj < 2; ++jj) {
        for (size_t ii = 0; ii < 6; ++ii) {
            if (joystick[jj] & (1 << ii)) {
                keys[cpcJoystick[jj][ii].row] &= ~cpcJoystick[jj][ii].key;
            }
        }
    }
}

void CPC::setBrand(uint_fast8_t brandNumber) {

    brand = brandNumber & 0x7;
}

void CPC::setSoundRate(uint_fast32_t frame, bool syncToVideo) {

    double value = static_cast<double>(BASE_CLOCK_CPC) / static_cast<double>(SAMPLE_RATE);

    if (syncToVideo) {
        double factor = static_cast<double>(FRAME_TIME_50HZ) / static_cast<double>(frame);
        value /= factor;
    }

    skip = static_cast<uint32_t>(value);
    tail = value - skip;
}
// vim: et:sw=4:ts=4
