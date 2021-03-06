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

#include "Spectrum.h"

#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    bus(0xFF),
    joystick(0),
    psgPresent{false, false, false, false},
    currentPsg(0),
    idle(0xFF),
    paging(0x0020), mask(0x0001),
    contendedPage{false, true, false, false},
    romPage{true, false, false, false},
    set48(true), rom48(true),
    stereo(StereoMode::STEREO_MONO) {

    // This is just for the laughs. We initialize the whole RAM to the
    // values that appeared in the Spectrum at boot time.
    for (size_t ii = 0; ii < (1 << 16); ii += 2) {
        *(reinterpret_cast<uint32_t*>(ram) + ii) = 0x00000000;
        *(reinterpret_cast<uint32_t*>(ram) + ii + 1) = 0xFFFFFFFF;
    }

    setPage(0, 0, true, false);
    setPage(1, 5, false, true);
    setPage(2, 2, false, false);
    setPage(3, 0, false, false);
    setScreenPage(5);
}

void Spectrum::loadRoms(RomVariant model) {
    ifstream ifs;
    vector<string> romNames;
    vector<string> romPaths;
    string romName;

    char* pHome = getenv(SPECIDE_HOME_ENV);

    romPaths.push_back("");
    if (pHome != nullptr) {
#if (SPECIDE_ON_UNIX==1)
        string home(pHome);
        home += string("/") + string(SPECIDE_CONF_DIR) + string("/roms/");
        romPaths.push_back(home);
#else
        string home(pHome);
        home += string("\\") + string(SPECIDE_CONF_DIR) + string("\\roms\\");
        romPaths.push_back(home);
#endif
    }
#if (SPECIDE_ON_UNIX==1)
    romPaths.push_back("/usr/local/share/spectrum-roms/");
    romPaths.push_back("/usr/share/spectrum-roms/");
#endif

    switch (model) {
        case RomVariant::ROM_48_EN:
            romNames.push_back("48.rom");
            break;

        case RomVariant::ROM_128_EN:
            romNames.push_back("128-0.rom");
            romNames.push_back("128-1.rom");
            break;

        case RomVariant::ROM_PLUS2_EN:
            romNames.push_back("plus2-0.rom");
            romNames.push_back("plus2-1.rom");
            break;

        case RomVariant::ROM_PLUS3_EN:
            romNames.push_back("plus3-0.rom");
            romNames.push_back("plus3-1.rom");
            romNames.push_back("plus3-2.rom");
            romNames.push_back("plus3-3.rom");
            break;

        case RomVariant::ROM_48_ES:
            romNames.push_back("48-spanish.rom");
            break;

        case RomVariant::ROM_128_ES:
            romNames.push_back("128-spanish-0.rom");
            romNames.push_back("128-spanish-1.rom");
            break;

        case RomVariant::ROM_PLUS2_ES:
            romNames.push_back("plus2-spanish-0.rom");
            romNames.push_back("plus2-spanish-1.rom");
            break;

        case RomVariant::ROM_PLUS3_ES:
            romNames.push_back("plus3-spanish-0.rom");
            romNames.push_back("plus3-spanish-1.rom");
            romNames.push_back("plus3-spanish-2.rom");
            romNames.push_back("plus3-spanish-3.rom");
            break;

        case RomVariant::ROM_PENTAGON:
            romNames.push_back("pentagon-0.rom");
            romNames.push_back("pentagon-1.rom");
            romNames.push_back("trdos.rom");
            break;

        default:
            romNames.push_back("48.rom");
            break;
    }

    size_t j = 0;
    bool fail = true;

    do {
        for (size_t i = 0; i < romNames.size(); ++i) {
            size_t pos = 0;
            romName = romPaths[j] + romNames[i];
            cout << "Trying ROM: " << romName << endl;
            ifs.open(romName, ifstream::binary);

            // If it fails, try the ROM in /usr/share/spectrum-roms
            fail = ifs.fail();
            if (fail)
                break;

            char c;
            while (ifs.get(c))
                rom[((2 << 14) * i) + pos++] = c;

            ifs.close();
        }
        ++j;
    } while (fail && j < romPaths.size());
}

void Spectrum::set128K() {

    spectrum128K = true;
    spectrumPlus2A = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgPresent[0] = true;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2() {

    spectrum128K = true;
    spectrumPlus2A = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgPresent[0] = true;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2A() {

    spectrum128K = false;
    spectrumPlus2A = true;
    plus3Disk = false;
    betaDisk128 = false;
    psgPresent[0] = true;
    mask = 0x0004;
    reset();
}

void Spectrum::setPlus3() {

    spectrum128K = false;
    spectrumPlus2A = true;
    plus3Disk = true;
    betaDisk128 = false;
    psgPresent[0] = true;
    mask = 0x0004;
    reset();
}

void Spectrum::setPentagon() {

    spectrum128K = true;
    spectrumPlus2A = false;
    plus3Disk = false;
    betaDisk128 = true;
    psgPresent[0] = true;
    mask = 0xFFFF;
    reset();
}

void Spectrum::clock() {

    // ULA is 'clocked' before Z80. This means:
    //
    // ULA   Z80
    // ---------
    //  0  | RST
    //  1  |  H
    //  2  |  L
    //  3  |  H
    //  4  |  L
    //
    // This is important, because when ULA is at 0 again Z80 clock will be
    // LOW. Contention intervals work this way.
    bool as_ = z80.c & SIGNAL_MREQ_;
    bool io_ = z80.c & SIGNAL_IORQ_;

    size_t memArea = z80.a >> 14;

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

    // If a contended RAM page is selected, we'll have memory contention.
    ula.contendedBank = contendedPage[memArea];

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    // I've found that separating both data buses is helpful for all
    // Speccies.
    bus_1 = bus;

    bool contendedAccess = contendedPage[memArea] && !as_;
    if (!ula.mem) {
        // Snow effect. ULA::snow is always false for +2A/+3/Pentagon
        if (ula.snow && contendedAccess) {
            if (memArea == 1) {
                bus = scr[(ula.a & 0x3F80) | (z80.a & 0x007F)];
            } else {
                bus = sno[(ula.a & 0x3F80) | (z80.a & 0x007F)];
            }
        } else {
            bus = scr[ula.a];
        }
    } else if (!spectrumPlus2A || contendedAccess) {
        // For +2A/+3 machines, the Gate Array stores all bytes that pass
        // though it. This means, any contended access will alter this byte.
        // For other machines, this byte is altered with each access.
        bus = z80.d;
    }

    ula.d = bus;
    ula.clock();

    z80.c = ula.z80_c;

    ++count;
    if (!(count & 0x03)) {
        ula.beeper();
        psgClock();
        filter[index] = covox;
        index = (index + 1) % FILTER_BZZ_SIZE;

        if (!(count & 0x07)) {
            if (plus3Disk) fdc765.clock();
            //if (betaDisk128) fd1793.clock();
        }
    }

    // We clock the Z80 if the ULA allows.
    if (ula.cpuClock) {
        // Z80 gets data from the ULA or memory, only when reading.
        if (z80.access) {
            if (!io_) {
                // 48K/128K/Plus2 floating bus. Return idle status by default,
                // or screen data, if the ULA is working.
                if (z80.rd) {
                    z80.d = (ula.idle) ? idle : bus & idle;
                }

                // 128K only ports (paging, disk)
                if (spectrum128K) {
                    if (!(z80.a & 0x8002)) {
                        if (z80.wr || z80.rd)
                            updatePage(0);
                    }
                } else if (spectrumPlus2A) {
                    switch (z80.a & 0xF002) {
                        case 0x0000:    // In +2A/+3 this is the floating bus port.
                            if (z80.rd) {
                                if (!(paging & 0x0020)) {
                                    z80.d = (bus_1 & idle) | 0x01;
                                }
                            }
                            break;
                        case 0x1000:    // 0x1FFD (+3 Paging High)
                            if (z80.wr) {
                                updatePage(1);
                            }
                            break;
                        case 0x2000:    // 0x2FFD (+3 FDC765 Main Status)
                            if (plus3Disk) {
                                if (z80.rd) {
                                    z80.d = fdc765.status();
                                }
                            }
                            break;
                        case 0x3000:    // 0x3FFD (+3 FDC765 Data)
                            if (plus3Disk) {
                                if (z80.wr) {
                                    fdc765.write(z80.d);
                                } else if (z80.rd) {
                                    z80.d = fdc765.read();
                                }
                            }
                            break;
                        case 0x4000: // fall-through
                        case 0x5000: // fall-through
                        case 0x6000: // fall-through
                        case 0x7000: // 0x7FFD (128K Paging / +3 Paging Low)
                            if (z80.wr) {
                                updatePage(0);
                            }
                            break;

                        default:
                            break;
                    }
                }

                // AY-3-8912 ports.
                if (psgPresent[0]) {    // If there are PSGs, there is a PSG 0
                    switch (z80.a & 0xC002) {
                        case 0x8000:    // 0xBFFD
                            if (z80.wr) {
                                psgWrite();
                            } else if (z80.rd && spectrumPlus2A) {
                                psgRead();
                            }
                            break;

                        case 0xC000:    // 0xFFFD
                            if (z80.wr) {
                                if ((z80.d & 0x98) == 0x98) {
                                    psgSelect();
                                } else {
                                    psgAddr();
                                }
                            } else if (z80.rd) {
                                psgRead();
                            }
                            break;

                        default:
                            break;
                    }
                }

                // Common ports.
                // Ports in the form XXXXXXXX 0XX11111 are blocked when TR-DOS
                // is active. This affects kempston joystick, for instance.
                if (betaDisk128 && (romBank == 0x0002)) {
                    if ((z80.a & 0x0003) == 0x0003) {
                        // uint_fast8_t fdAddr = (z80.a & 0xE0) >> 5;
                        // if (z80.rd) {
                            // z80.d = fd1793.read(fdAddr);
                        // } else if (z80.wr) {
                            // fd1793.write(fdAddr, z80.d);
                        // }
                    }
                } else {
                    if (kempston && !(z80.a & 0x0020)) {    // Kempston joystick.
                        if (z80.rd) {
                            z80.d = joystick;
                        }
                    }
                }

                if (!(z80.a & 0x0004)) {
                    if (z80.wr) {
                        covox = z80.d * 0x40;
                    }
                }

                if (!(z80.a & 0x0001)) {         // ULA port
                    if (z80.wr) {
                        ula.ioWrite(z80.d);
                    } else if (z80.rd) {
                        z80.d = ula.ioRead();
                    }
                }
            } else if (!as_) {
                // BetaDisk128 pages TR-DOS ROM when the PC is in the range
                // 0x3D00-0x3DFF and the 48K BASIC ROM is paged in.
                // Note that we're not considering romBank != (0, 1) because
                // BetaDisk128 is not allowed in Plus2A/Plus3 models.
                if (betaDisk128 && z80.fetch) {
                    if ((romBank == 0x0001) && ((z80.a & 0xFF00) == 0x3D00)) {
                        setPage(0, 2, true, false);
                    } else if (memArea) {
                        setPage(0, romBank, true, false);
                    }
                }

                if (z80.rd) {
                    z80.d = map[memArea][z80.a & 0x3FFF];
                } else if (!romPage[memArea] && z80.wr) {
                    map[memArea][z80.a & 0x3FFF] = z80.d;
                }
            }
        } else if (as_ && io_) {
            z80.d = 0xFF;
        }
        z80.clock();
    }
}

void Spectrum::updatePage(uint_fast8_t reg) {

    if (!(paging & 0x0020)) {
        if (reg == 1)
            paging = (z80.d << 8) | (paging & 0x00FF);
        else
            paging = z80.d | (paging & 0xFF00);

        // Update +3 disk drive(s) motor status.
        fdc765.motor(plus3Disk && (paging & 0x0800));

        // Select screen to display.
        setScreenPage(((paging & 0x0008) >> 2) | 0x05);
        setSnowPage(paging & 0x0005);

        if (paging & 0x0100) {      // Special paging mode.
            switch (paging & 0x0600) {
                case 0x0000:
                    setPage(0, 0, false, false);
                    setPage(1, 1, false, false);
                    setPage(2, 2, false, false);
                    setPage(3, 3, false, false);
                    break;
                case 0x0200:
                    setPage(0, 4, false, true);
                    setPage(1, 5, false, true);
                    setPage(2, 6, false, true);
                    setPage(3, 7, false, true);
                    break;
                case 0x0400:
                    setPage(0, 4, false, true);
                    setPage(1, 5, false, true);
                    setPage(2, 6, false, true);
                    setPage(3, 3, false, false);
                    break;
                case 0x0600:
                    setPage(0, 4, false, true);
                    setPage(1, 7, false, true);
                    setPage(2, 6, false, true);
                    setPage(3, 3, false, false);
                    break;

                default:
                    assert(false);
            }
        } else {                    // Normal paging mode.
            ramBank = paging & 0x0007;
            romBank =
                ((paging & 0x0010) >> 4) | ((paging & 0x0400) >> 9);

            setPage(0, romBank, true, false);
            setPage(1, 5, false, true);
            setPage(2, 2, false, false);
            setPage(3, ramBank, false, ((ramBank & mask) == mask));

            rom48 = ((spectrumPlus2A && romBank == 3)
                    || (spectrum128K && romBank == 1));
            set48 = (paging & 0x0020);
        }
    }
}

void Spectrum::reset() {

    ula.reset();    // Synchronize clock level.
    z80.reset();
    psgReset();
    fdc765.reset();

    covox = 0;
    romBank = 0;
    ramBank = 0;
    setPage(0, 0, true, false);
    setPage(1, 5, false, true);
    setPage(2, 2, false, false);
    setPage(3, 0, false, false);
    setScreenPage(5);

    if (spectrum128K || spectrumPlus2A) {
        paging = 0x0000;
        set48 = false;
        rom48 = false;
    } else {
        paging = 0x0020;
        set48 = true;
        rom48 = true;
    }
}

void Spectrum::psgSelect() {

    size_t newPsg = (~z80.d) & 0x03;
    if (psgPresent[newPsg]) {
        currentPsg = newPsg;
        psg[currentPsg].lchan = (z80.d & 0x40);
        psg[currentPsg].rchan = (z80.d & 0x20);
    }
}

void Spectrum::psgRead() {

    if (psgPresent[currentPsg])
        z80.d = psg[currentPsg].read();
}

void Spectrum::psgWrite() {

    if (psgPresent[currentPsg])
        psg[currentPsg].write(z80.d);
}

void Spectrum::psgAddr() {

    if (psgPresent[currentPsg])
        psg[currentPsg].addr(z80.d);
}

void Spectrum::psgReset() {

    for (size_t ii = 0; ii < 4; ++ii) {
        psg[ii].reset();
        psg[ii].seed = 0xFFFF - (ii * 0x1111);
    }
}

void Spectrum::psgClock() {

    if (psgPresent[0]) {
        psg[0].clock();
        if (psgPresent[1]) {
            psg[1].clock();
            if (psgPresent[2]) psg[2].clock();
            if (psgPresent[3]) psg[3].clock();
        }
    }
}

void Spectrum::psgPlaySound(bool play) {

    psg[0].playSound = play;
    psg[1].playSound = play;
    psg[2].playSound = play;
    psg[3].playSound = play;
}

void Spectrum::psgSample() {

    if (psgPresent[0]) {
        psg[0].sample();
        if (psgPresent[1]) {
            psg[1].sample();
            if (psgPresent[2]) psg[2].sample();
            if (psgPresent[3]) psg[3].sample();
        }
    }
}

void Spectrum::psgChip(bool aychip) {

    psg[0].setVolumeLevels(aychip);
    psg[1].setVolumeLevels(aychip);
    psg[2].setVolumeLevels(aychip);
    psg[3].setVolumeLevels(aychip);
}

void Spectrum::sample() {

    l = r = ula.sample() + dac();
    psgSample();

    switch (stereo) {
        case StereoMode::STEREO_ACB: // ACB
            l += psg[0].channelA;
            l += psg[0].channelC;
            r += psg[0].channelB;
            r += psg[0].channelC;
            break;

        case StereoMode::STEREO_ABC: // ABC
            l += psg[0].channelA;
            l += psg[0].channelB;
            r += psg[0].channelB;
            r += psg[0].channelC;
            break;

        case StereoMode::STEREO_TURBO_MONO: // TurboSound with 2 PSGs, mono.
            l += psg[0].channelA;
            l += psg[0].channelB;
            l += psg[0].channelC;
            r += psg[0].channelA;
            r += psg[0].channelB;
            r += psg[0].channelC;

            l += psg[1].channelA;
            l += psg[1].channelB;
            l += psg[1].channelC;
            r += psg[1].channelA;
            r += psg[1].channelB;
            r += psg[1].channelC;
            break;

        case StereoMode::STEREO_TURBO_ACB:  // TurboSound with 2 PSGs, ACB
            l += psg[0].channelA;
            l += psg[0].channelC;
            r += psg[0].channelB;
            r += psg[0].channelC;

            l += psg[1].channelA;
            l += psg[1].channelC;
            r += psg[1].channelB;
            r += psg[1].channelC;
            break;

        case StereoMode::STEREO_TURBO_ABC: // TurboSound with 2 PSGs, ABC
            l += psg[0].channelA;
            l += psg[0].channelB;
            r += psg[0].channelB;
            r += psg[0].channelC;

            l += psg[1].channelA;
            l += psg[1].channelB;
            r += psg[1].channelB;
            r += psg[1].channelC;
            break;

        case StereoMode::STEREO_NEXT:
            for (size_t ii = 0; ii < 4; ++ii) {
                if (psg[ii].lchan) {
                    l += psg[ii].channelA >> 1;
                    l += psg[ii].channelB >> 1;
                    l += psg[ii].channelC >> 1;
                }
                if (psg[ii].rchan) {
                    r += psg[ii].channelA >> 1;
                    r += psg[ii].channelB >> 1;
                    r += psg[ii].channelC >> 1;
                }
            }
            break;

        default:    // mono, all channels go through both sides.
            l += psg[0].channelA;
            l += psg[0].channelB;
            l += psg[0].channelC;
            r += psg[0].channelA;
            r += psg[0].channelB;
            r += psg[0].channelC;
            break;
    }
}

int Spectrum::dac() {

    int sound = 0;
    for (size_t i = 0; i < FILTER_BZZ_SIZE; ++i) {
        sound += filter[i];
    }
    sound /= FILTER_BZZ_SIZE;
    return sound;
}

void Spectrum::setPage(uint_fast8_t page,
        uint_fast8_t bank, bool isRom, bool isContended) {

    size_t addr = bank * (2 << 14);
    map[page] = (isRom) ? &rom[addr] : &ram[addr];
    romPage[page] = isRom;
    contendedPage[page] = isContended;
}

void Spectrum::setScreenPage(uint_fast8_t page) {

    scr = &ram[page * (2 << 14)];
}

void Spectrum::setSnowPage(uint_fast8_t page) {

    sno = &ram[page * (2 << 14)];
}

// vim: et:sw=4:ts=4
