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

void CPC::loadExtensionRoms() {

    vector<string> romDirs = getRomDirs();

    for (size_t romSlot = 1; romSlot < 16; romSlot++) {
        size_t j = 0;
        bool fail = true;
        if (romNames[romSlot].empty()) {
            extPresent[romSlot] = false;
        } else do {
            string romPath = romDirs[j] + romNames[romSlot];
            cout << "Trying ROM in Slot #"
                << static_cast<uint32_t>(romSlot) << ": " << romPath << "... ";
            ifstream ifs(romPath, ifstream::binary);
            fail = ifs.fail() || ifs.read(reinterpret_cast<char*>(ext[romSlot]), 0x4000).fail();
            extPresent[romSlot] = !fail;
            cout << string(fail ? "FAIL" : "OK") << endl;
            ++j;
        } while (fail && j < romDirs.size());
    }
    for (size_t romSlot = 16; romSlot < 256; ++romSlot) {
        extPresent[romSlot] = false;
    }
}

void CPC::set464() {

    cpc128K = false;
    cpcDisk = false;

    loadRoms(RomVariant::ROM_CPC464);
    loadExtensionRoms();
    reset();
}

void CPC::set664() {

    cpc128K = false;
    cpcDisk = true;

    romNames[0x07] = "amsdos.rom";

    loadRoms(RomVariant::ROM_CPC664);
    loadExtensionRoms();
    reset();
}

void CPC::set6128() {

    cpc128K = true;
    cpcDisk = true;

    romNames[0x07] = "amsdos.rom";

    loadRoms(RomVariant::ROM_CPC6128);
    loadExtensionRoms();
    reset();
}

void CPC::run() {

    //static double remaining = 0;

    while (!ga.sync) {

        clock();

        /*if (tape.playing) {
            if (!tape.sample--) {
                ula.tapeIn = tape.advance() | 0x80;
            }
        } else {
            ula.tapeIn &= 0x7F;
        }

        // Generate sound. This maybe can be done using the same counter?
        if (!(--skipCycles)) {
            skipCycles = skip;
            remaining += tail;
            if (remaining >= 1.0) {
                ++skipCycles;
                remaining -= 1.0;
            }

            sample();
        }
        */
    }
    scanKeys();
    ga.sync = false;
}

void CPC::clock() {

    bool m1_ = z80.c & SIGNAL_M1_;
    bool as_ = z80.c & SIGNAL_MREQ_;
    bool io_ = z80.c & SIGNAL_IORQ_;
    bool rd_ = z80.c & SIGNAL_RD_;
    bool wr_ = z80.c & SIGNAL_WR_;
    size_t memArea = z80.a >> 14;

    ga.d = mem[ga.crtc.pageAddress][ga.crtc.byteAddress | ga.cClkOffset()];

    // First we clock the Gate Array. Further clocks will be generated here.
    ga.clock();

    if (ga.crtcClock()) {
        ppi.portB = 0x5E | (ga.crtc.vSync ? 1 : 0);
    }

    if (ga.psgClock()) {
        psg.setPortA(keys[ppi.portC & 0x0F]);

        // Execute PSG command.
        switch (ppi.portC & 0xC0) {
            case 0x40:
                ppi.portA = ppi.inputA ? psg.read() : 0xFF;
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

        psg.clock();
    }
    /*
       ++count;
       if (cpcDisk && !(count & 0x07))
       fdc.clock();
   */

    // Z80 gets data from the ULA or memory, only when reading.
    if (!io_) {
        // Gate Array. This is accessible by both I/O reads and I/O writes.
        if (m1_ && (z80.a & 0xC000) == 0x4000) {
            if ((z80.d & 0xC0) == 0xC0) {
                selectRam(z80.d);
            } else {
                ga.write(z80.d);
            }
        }

        if (m1_ && (z80.a & 0x2000) == 0x0000) {
            romBank = z80.d;
            if (extPresent[romBank]) {
                hiRom = ext[romBank];
            } else {
                hiRom = &rom[0x4000];
            }

        }
    }

    if (ga.cpuClock()) {

        z80.c = ga.z80_c;

        if (!io_) {
            // CRTC
            if ((z80.rd || z80.wr) && ((z80.a & 0x4000) == 0x0000)) {
                switch (z80.a & 0x0300) {
                    case 0x0000:    // CRTC Register Select (WO) at &BC00
                        ga.crtc.wrAddress(z80.d);
                        break;
                    case 0x0100:    // CRTC Register Write (WO) at &BD00
                        ga.crtc.wrRegister(z80.d);
                        break;
                    case 0x0200:    // CRTC Status Register Read (only type 1)
                        z80.d = ga.crtc.rdStatus();
                        break;
                    case 0x0300:    // CRTC Register Read (RO)
                        z80.d = ga.crtc.rdRegister();
                        break;
                    default:
                        break;
                }
            }

            // PPI
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
            }

            /*
            // Peripherals
            if ((z80.a & 0x0400) == 0x0000) {
            // FDC
            if ((z80.a & 0x0080) == 0x0000) {
            switch (z80.a & 0x0101) {
            case 0x0000:    // Drive A motor
            break;
            case 0x0001:    // Drive B motor
            break;
            case 0x0100:    // FDC main status register
            break;
            case 0x0101:    // FDC data register
            break;
            default:
            break;
            }
            }
            }

            */
        } else if (!as_) {
            if (!rd_) {
                switch (memArea) {
                    case 0:
                        z80.d = ga.lowerRom ? rom[z80.a & 0x3FFF] : mem[0][z80.a & 0x3FFF];
                        break;
                    case 3:
                        z80.d = ga.upperRom ? rom[(z80.a & 0x3FFF) | 0x4000] : mem[3][z80.a & 0x3FFF];
                        break;
                    default:
                        z80.d = mem[memArea][z80.a & 0x3FFF];
                        break;
                }
            } else if (!wr_) {
                mem[memArea][z80.a & 0x3FFF] = z80.d;
            }
        }

        z80.clock();
        ga.z80_c = z80.c;
    }
}

void CPC::reset() {

    z80.reset();
    psgReset();
//    fdc.reset();
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

void CPC::sample(int& l, int& r) {

    //buzzer.sample();
    psg.sample();
    //l = r = buzzer.signal;
    l = r = 0;

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

    for (size_t ii = 0; ii < 16; ++ii) {
        keys[ii] = 0xFF;
    }

    for (size_t ii = 0; ii < sizeof(cpcKeys) / sizeof(KeyBinding); ++ii) {
        if (Keyboard::isKeyPressed(cpcKeys[ii].keyName)) {
            keys[cpcKeys[ii].row] &= ~cpcKeys[ii].key;
        }
    }
}
// vim: et:sw=4:ts=4
