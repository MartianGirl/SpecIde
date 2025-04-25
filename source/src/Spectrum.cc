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

#include "SpecIde.h"
#include "Spectrum.h"

#include <cstdlib>
#include <ctime>

Spectrum::Spectrum() :
    contendedPage{false, true, false, false},
    romPage{true, false, false, false} {

    // This is just for the laughs. We initialize the whole RAM to the
    // values that appeared in the Spectrum at boot time.
    for (size_t ii = 0; ii < (1 << 15); ii += 2) {
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

    vector<string> romNames;
    vector<string> romDirs = getRomDirs();

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
            string romName = romDirs[j] + romNames[i];
            cout << "Trying ROM: " << romName << "... ";

            ifstream ifs(romName, ifstream::binary);
            char* bufaddr = reinterpret_cast<char*>(&rom[(1 << 14) * i]);
            // Logical || is a short circuit operator.
            fail = ifs.fail() || ifs.read(bufaddr, 0x4000).fail();
            cout << string(fail ? "FAIL" : "OK") << endl;
        }
        ++j;
    } while (fail && j < romDirs.size());
}

void Spectrum::setIssue2(RomVariant variant) {

    spectrum128K = false;
    spectrumPlus2A = false;
    pentagon = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgChips = 0;
    mask = 0x0001;
    ula.setUlaVersion(0);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_48K, sync);

    reset();
}

void Spectrum::setIssue3(RomVariant variant) {

    spectrum128K = false;
    spectrumPlus2A = false;
    pentagon = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgChips = 0;
    mask = 0x0001;
    ula.setUlaVersion(1);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_48K, sync);

    reset();
}

void Spectrum::set128K(RomVariant variant) {

    spectrum128K = true;
    spectrumPlus2A = false;
    pentagon = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgChips = 1;
    mask = 0x0001;
    ula.setUlaVersion(2);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_128K, sync);

    reset();
}

void Spectrum::setPlus2(RomVariant variant) {

    spectrum128K = true;
    spectrumPlus2A = false;
    pentagon = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgChips = 1;
    mask = 0x0001;
    ula.setUlaVersion(3);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_128K, sync);

    reset();
}

void Spectrum::setPlus2A(RomVariant variant) {

    spectrum128K = false;
    spectrumPlus2A = true;
    pentagon = false;
    plus3Disk = false;
    betaDisk128 = false;
    psgChips = 1;
    mask = 0x0004;
    ula.setUlaVersion(4);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_128K, sync);

    reset();
}

void Spectrum::setPlus3(RomVariant variant) {

    spectrum128K = false;
    spectrumPlus2A = true;
    pentagon = false;
    plus3Disk = true;
    betaDisk128 = false;
    psgChips = 1;
    mask = 0x0004;
    ula.setUlaVersion(4);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_128K, sync);

    fdc765.clockFrequency = 1.0;  // Relative to 1MHz

    reset();
}

void Spectrum::setPentagon(RomVariant variant) {

    spectrum128K = true;
    spectrumPlus2A = false;
    pentagon = true;
    plus3Disk = false;
    betaDisk128 = true;
    psgChips = 1;
    mask = 0xFFFF;
    ula.setUlaVersion(5);

    loadRoms(variant);
    setSoundRate(SoundRate::SOUNDRATE_PENTAGON, sync);

    reset();
}

void Spectrum::run() {

    static uint_fast32_t accumulator = 0;

    while (!ula.vSync) {
        if (flashTap) {
            checkTapeTraps();
        }

        clock();

        if (tape.playing) {
            if (!tape.sample--) {
                tape.advance();
                ula.setEarLevel(tape.level & 0x40, tape.playing);
            }
        }

        accumulator += SAMPLE_RATE;
        if (accumulator > clockRate) {
            accumulator -= clockRate;
            sample();
        }
    }

    ula.vSync = false;
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

    uint_fast32_t memArea = z80.a >> 14;

    // First we clock the ULA. This generates video and contention signals.
    // We need to provide the ULA with the Z80 address and control buses.
    ula.z80_a = z80.a;
    ula.z80_c = z80.c;

    // If a contended RAM page is selected, we'll have memory contention.
    ula.contendedBank = contendedPage[memArea];

    // ULA gets the data from memory or Z80, or outputs data to Z80.
    // I've found that separating both data buses is helpful for all
    // Speccies.
    gateArrayByte = bus;

    switch (ula.snow) {
        case SNOW:  // 1st ULA burst: CAS loads R register
            if (contendedPage[memArea] && z80.state == Z80State::ST_OCF_T3L_RFSH1) {
                snowMode = SNOW;
                snowAddr = z80.a & 0x007f;
                snowArea = memArea;
            }
            break;
        case DUPL:  // 2nd ULA burst: CAS loads previous column address
            if (contendedPage[memArea] && z80.state == Z80State::ST_OCF_T3L_RFSH1) {
                snowMode = DUPL;
                snowAddr = ula.a & 0x007e;
                snowArea = memArea;
            }
            break;
        case ENDS:  // End of the SNOW cycle.
            if (snowMode == SNOW) {
                snowMode = NONE;
                snowAddr = ula.a & 0x007f;
            }
            break;
        case ENDD:  // End of the DUPL cycle.
            snowMode = NONE;
            snowAddr = ula.a & 0x007f;
            break;
    }

    if (!ula.mem) {
        // Snow effect. ULA::snow is always false for +2A/+3/Pentagon
        if (snowMode) {
            snowAddr = (ula.a & 0x3F80) | (snowAddr & 0x007F);
            bus = (snowArea != 3 || snowMode == DUPL) ? scr[snowAddr] : sno[snowAddr];
        } else {
            bus = scr[ula.a];
        }
    } else if (!spectrumPlus2A || (contendedPage[memArea] && !as_)) {
        // For +2A/+3 machines, the Gate Array stores all bytes that pass
        // though it. This means, any contended access will alter this byte.
        // For other machines, this byte is altered with each access.
        bus = z80.d;
    }

    ula.d = bus;
    ula.clock();

    z80.c = ula.z80_c;

    // Count is only used for its less significant bits, so there is no
    // overflow risk even with 32 bit types.
    ++count;

    if (!(count & 0x03)) {
        ula.beeper();
        psgClock();

        for (int c = 0; c < 4; ++c) {
            filter[c].add(covox[c]);
        }

        if (joystick == JoystickType::FULLER) {
            fullerCount += psgPeriod;
            if (fullerCount > fullerPeriod) {
                fullerCount -= fullerPeriod;
                psg[4].clock();
            }
        }
    }

    if (!(count % 0x07)) {
        if (plus3Disk) fdc765.clock();
        //if (betaDisk128) fd1793.clock();
    }

    // Switch pages only if the ULA is not accessing memory.
    if (switchPage && allowPageChange()) {
        updatePage();
        switchPage = false;
    }

    // We clock the Z80 if the ULA allows.
    if (ula.cpuClock) {
        // Classifying the accesses based on z80.state saves one condition.
        if (z80.state == Z80State::ST_OCF_T2L_DATARD) { // Opcode fetches
            if (betaDisk128) {
                // BetaDisk128 pages TR-DOS ROM when the PC is in the range
                // 0x3D00-0x3DFF and the 48K BASIC ROM is paged in.
                // Note that we're not considering romBank != (0, 1) because
                // BetaDisk128 is not allowed in Plus2A/Plus3 models.
                if ((romBank == 0x0001) && ((z80.a & 0xFF00) == 0x3D00)) {
                    setPage(0, 2, true, false);
                } else if (memArea) {
                    setPage(0, romBank, true, false);
                }
            }
            z80.d = mem[memArea][z80.a & 0x3FFF];
        } else if (z80.state == Z80State::ST_MEMRD_T3H_DATARD) { // Mem reads
            z80.d = mem[memArea][z80.a & 0x3FFF];
        } else if (z80.state == Z80State::ST_MEMWR_T3L_DATAWR) { // Mem writes
            if (!romPage[memArea]) { mem[memArea][z80.a & 0x3FFF] = z80.d; }
        } else if (z80.state == Z80State::ST_IORD_T3L_DATARD) { // I/O reads.
            // Unattached port read by default (floating bus / idle bus).
            z80.d = ula.idle ? idle : bus & idle;

            // ULA port goes first because it's on a separate bus.
            // Other peripherals on the Z80 bus will overwrite this value.
            if (!(z80.a & 0x0001)) { z80.d = ula.ioRead(); }

            // Spectrum 128K ports.
            if (spectrum128K) {
                if (!(z80.a & 0x8002)) { selectPage(0); }   // 0x7FFD (128K paging is changed also on reads)
            } else if (spectrumPlus2A) {
                switch (z80.a & 0xF002) {
                    case 0x0000:    // 0x0FFD (Printer port - In +2A/+3 this is the floating bus port)
                        if (!(pageRegs & 0x0020)) { z80.d = (gateArrayByte & idle) | 0x01; }
                        break;
                    case 0x2000:    // 0x2FFD (+3 FDC765 Main Status)
                        if (plus3Disk) { z80.d = fdc765.status(); }
                        break;
                    case 0x3000:    // 0x3FFD (+3 FDC765 Data)
                        if (plus3Disk) { z80.d = fdc765.read(); }
                        break;
                    default:
                        break;
                }
            }

            // AY-3-8912 ports.
            if (psgChips) {
                switch (z80.a & 0xC002) {
                    case 0x8000: // 128K AY Data Port
                        if (spectrumPlus2A) { psgRead(); }
                        break;
                    case 0xC000: // 128K AY Control Port
                        psgRead();
                        break;
                    default:
                        break;
                }
            }

            // Common ports.
            // Ports in the form XXXXXXXX 0XX11111 are blocked when TR-DOS
            // is active. This affects kempston joystick, for instance.
            if (!betaDisk128 || (romBank != 0x0002)) {
                switch (joystick) {
                    case JoystickType::KEMPSTON_OLD:
                    case JoystickType::CURSOR:  // fall-through
                                                // If the joystick type is CURSOR, the second
                                                // joystick is mapped to KEMPSTON.
                        if (!(z80.a & 0x0020)) { z80.d = kempstonData; }
                        break;
                    case JoystickType::KEMPSTON_NEW:
                        if (!(z80.a & 0x00E0)) { z80.d = kempstonData; }
                        break;
                    case JoystickType::FULLER:
                        switch (z80.a & 0x00F0) {
                            case 0x0030:
                                // Port 0x003F, Fuller AY control port
                                z80.d = psg[4].read(); break;
                            case 0x0050:
                                // Port 0x005F, Fuller AY data port
                                z80.d = psg[4].read(); break;
                            case 0x0070:
                                // Port 0x007F, Fuller joystick port
                                z80.d = fullerData; break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            } else if ((z80.a & 0x0003) == 0x0003) {
                // uint_fast8_t fdAddr = (z80.a & 0xE0) >> 5;
                // if (z80.rd) {
                // z80.d = fd1793.read(fdAddr);
                // } else if (z80.wr) {
                // fd1793.write(fdAddr, z80.d);
                // }
            }
        } else if (z80.state == Z80State::ST_IOWR_T2L_IORQ) { // I/O writes.
            // 128K only ports (pagination, disk)
            if (spectrum128K) {
                if (!(z80.a & 0x8002)) { selectPage(0); }
            } else if (spectrumPlus2A) {
                switch (z80.a & 0xF002) {
                    case 0x1000:    // 0x1FFD (+3 High Page Register)
                        selectPage(1);
                        break;
                    case 0x3000:    // 0x3FFD (+3 FDC765 Data)
                        if (plus3Disk) { fdc765.write(z80.d); }
                        break;
                    case 0x4000: // fall-through
                    case 0x5000: // fall-through
                    case 0x6000: // fall-through
                    case 0x7000: // 0x7FFD (128K Page Register / +3 Low Page Register)
                        selectPage(0);
                        break;
                    default:
                        break;
                }
            }

            // AY-3-8912 ports.
            if (psgChips) {
                switch (z80.a & 0xC002) {
                    case 0x8000:
                        // 128K AY Data Port
                        psgWrite();
                        break;
                    case 0xC000:
                        // 128K AY Control Port
                        if ((z80.d & 0x98) == 0x98) { psgSelect(); } else { psgAddr(); }
                        break;
                    default:
                        break;
                }
            }

            // Common ports.
            if (!betaDisk128 || (romBank != 0x0002)) {
                switch (joystick) {
                    case JoystickType::KEMPSTON_OLD:
                    case JoystickType::CURSOR:  // fall-through
                                                // If the joystick type is CURSOR, the second
                                                // joystick is mapped to KEMPSTON.
                                                // Old Kempston joystick returns its value also on writes.
                        if (!(z80.a & 0x0020)) { z80.d = kempstonData; }
                        break;
                    case JoystickType::FULLER:
                        switch (z80.a & 0x00F0) {
                            case 0x0030:
                                // Port 0x003F, Fuller AY control port
                                psg[4].addr(z80.d); break;
                            case 0x0050:
                                // Port 0x005F, Fuller AY data port
                                psg[4].write(z80.d); break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            } else {
                // Ports in the form XXXXXXXX 0XX11111 are blocked when TR-DOS
                // is active. This affects kempston joystick, for instance.
                if ((z80.a & 0x0003) == 0x0003) {
                    // uint_fast8_t fdAddr = (z80.a & 0xE0) >> 5;
                    // if (z80.rd) {
                    // z80.d = fd1793.read(fdAddr);
                    // } else if (z80.wr) {
                    // fd1793.write(fdAddr, z80.d);
                    // }
                }
            }

            switch (covoxMode) {
                case Covox::MONO:
                    if ((z80.a & 0x00FF) == 0x00FB) {
                        covox[0] = covox[1] = covox[2] = covox[3] = z80.d * COVOX_VOLUME;
                    }
                    break;
                case Covox::STEREO:
                    if ((z80.a & 0x00FF) == 0xFB) { covox[0] = covox[1] = z80.d * COVOX_VOLUME; }
                    if ((z80.a & 0x00FF) == 0x4F) { covox[2] = covox[3] = z80.d * COVOX_VOLUME; }
                    break;
                case Covox::CZECH:
                    if ((z80.a & 0x009F) == 0x1F) {
                        switch (z80.a & 0x60) {
                            case 0x00: covox[0] = z80.d * COVOX_VOLUME; break;
                            case 0x20: covox[3] = z80.d * COVOX_VOLUME; break;
                            case 0x40: covox[1] = covox[2] = z80.d * COVOX_VOLUME; break;
                            default: break;
                        }
                    }
                    break;
                case Covox::SOUNDRIVE1:
                    if ((z80.a & 0x00AF) == 0x000F) {
                        switch (z80.a & 0x0050) {
                            case 0x00: covox[0] = z80.d * COVOX_VOLUME; break;
                            case 0x10: covox[1] = z80.d * COVOX_VOLUME; break;
                            case 0x40: covox[2] = z80.d * COVOX_VOLUME; break;
                            case 0x50: covox[3] = z80.d * COVOX_VOLUME; break;
                            default: break;
                        }
                    }
                    break;
                case Covox::SOUNDRIVE2:
                    if ((z80.a & 0x00F1) == 0x00F1) {
                        switch (z80.a & 0x000A) {
                            case 0x0: covox[0] = z80.d * COVOX_VOLUME; break;
                            case 0x2: covox[1] = z80.d * COVOX_VOLUME; break;
                            case 0x8: covox[2] = z80.d * COVOX_VOLUME; break;
                            case 0xA: covox[3] = z80.d * COVOX_VOLUME; break;
                            default: break;
                        }
                    }
                    break;
                default:
                    break;
            }

            // Writes to ULA go last, to account for the case where a
            // peripheral responds both to reads and writes, and uses
            // an even address.
            if (!(z80.a & 0x0001)) { ula.ioWrite(z80.d); }
        } else if (as_ && io_) { // MREQ and IORQ are low on more than the states considered here.
            z80.d = 0xFF;
        }

        z80.clock();
    }
}

void Spectrum::selectPage(uint_fast8_t reg) {

    if (!(pageRegs & 0x0020)) {
        if (reg == 1) {
            pageRegs = (z80.d << 8) | (pageRegs & 0x00FF);
        } else {
            pageRegs = (pageRegs & 0xFF00) | z80.d;
        }

        // Update +3 disk drive(s) motor status.
        fdc765.motor(plus3Disk && (pageRegs & 0x0800));

        switchPage = true;
    }
}

void Spectrum::updatePage() {

    // Select screen to display.
    uint_fast16_t screen = (pageRegs & 0x8) >> 2;
    setScreenPage(screen | 0x05);
    setSnowPage((pageRegs & 0x0005) | screen);

    if (pageRegs & 0x0100) {      // Special pagination mode.
        switch (pageRegs & 0x0600) {
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
    } else {                    // Normal pagination mode.
        ramBank = pageRegs & 0x0007;
        romBank = ((pageRegs & 0x0010) >> 4) | ((pageRegs & 0x0400) >> 9);

        setPage(0, romBank, true, false);
        setPage(1, 5, false, true);
        setPage(2, 2, false, false);
        setPage(3, ramBank, false, ((ramBank & mask) == mask));

        rom48 = ((spectrumPlus2A && romBank == 3)
                || (spectrum128K && romBank == 1));
        tape.is48K = set48 = (pageRegs & 0x0020);
    }
}

void Spectrum::reset() {

    ula.reset();    // Synchronize clock level.
    z80.reset();
    psgReset();
    fdc765.reset();

    covox[0] = covox[1] = covox[2] = covox[3] = 0;
    romBank = 0;
    ramBank = 0;
    setPage(0, 0, true, false);
    setPage(1, 5, false, true);
    setPage(2, 2, false, false);
    setPage(3, 0, false, false);
    setScreenPage(5);
    setSnowPage(5);

    if (spectrum128K || spectrumPlus2A) {
        pageRegs = 0x0000;
        tape.is48K = set48 = false;
        rom48 = false;
    } else {
        pageRegs = 0x0020;
        tape.is48K = set48 = true;
        rom48 = true;
    }

    snowMode = NONE;
    snowAddr = 0x0000;
}

void Spectrum::psgSelect() {

    size_t newPsg = (~z80.d) & 0x03;
    if (newPsg < psgChips) {
        currentPsg = newPsg;
        psg[currentPsg].lchan = (z80.d & 0x40);
        psg[currentPsg].rchan = (z80.d & 0x20);
    }
}

void Spectrum::psgRead() {

    if (currentPsg < psgChips) {
        z80.d = psg[currentPsg].read();
    }
}

void Spectrum::psgWrite() {

    if (currentPsg < psgChips) {
        psg[currentPsg].write(z80.d);
    }
}

void Spectrum::psgAddr() {

    if (currentPsg < psgChips) {
        psg[currentPsg].addr(z80.d);
    }
}

void Spectrum::psgReset() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].reset();
        psg[ii].seed = 0xFFFF - (ii * 0x1111);
    }

    if (joystick == JoystickType::FULLER) {
        psg[4].reset();
        psg[4].seed = 0xFFFF - (4 * 0x1111);
    }
}

void Spectrum::psgClock() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].clock();
    }
}

void Spectrum::psgPlaySound(bool play) {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].playSound = play;
    }

    if (joystick == JoystickType::FULLER) {
        psg[4].playSound = play;
    }
}

void Spectrum::psgSample() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].sample();
    }

    if (joystick == JoystickType::FULLER) {
        psg[4].sample();
    }
}

void Spectrum::psgChip(bool aychip) {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].setVolumeLevels(aychip);
    }

    if (joystick == JoystickType::FULLER) {
        psg[4].setVolumeLevels(aychip);
    }
}

void Spectrum::sample() {

    int l = ula.sample();
    int r = l;
    l += dac(0) + dac(1);
    r += dac(2) + dac(3);
    psgSample();

    if (joystick == JoystickType::FULLER) {
        l -= psg[4].channelA + psg[4].channelB + psg[4].channelC;
        r -= psg[4].channelA + psg[4].channelB + psg[4].channelC;
    }

    switch (stereo) {
        case StereoMode::STEREO_ACB: // ACB
            l -= psg[0].channelA + psg[0].channelC;
            r -= psg[0].channelB + psg[0].channelC;
            break;

        case StereoMode::STEREO_ABC: // ABC
            l -= psg[0].channelA + psg[0].channelB;
            r -= psg[0].channelB + psg[0].channelC;
            break;

        case StereoMode::STEREO_TURBO_MONO: // TurboSound with 2 PSGs, mono.
            l -= psg[0].channelA + psg[0].channelB + psg[0].channelC;
            l -= psg[1].channelA + psg[1].channelB + psg[1].channelC;
            r -= psg[0].channelA + psg[0].channelB + psg[0].channelC;
            r -= psg[1].channelA + psg[1].channelB + psg[1].channelC;
            break;

        case StereoMode::STEREO_TURBO_ACB:  // TurboSound with 2 PSGs, ACB
            l -= psg[0].channelA + psg[0].channelC;
            l -= psg[1].channelA + psg[1].channelC;
            r -= psg[0].channelB + psg[0].channelC;
            r -= psg[1].channelB + psg[1].channelC;
            break;

        case StereoMode::STEREO_TURBO_ABC: // TurboSound with 2 PSGs, ABC
            l -= psg[0].channelA + psg[0].channelB;
            l -= psg[1].channelA + psg[1].channelB;
            r -= psg[0].channelB + psg[0].channelC;
            r -= psg[1].channelB + psg[1].channelC;
            break;

        case StereoMode::STEREO_NEXT:
            for (size_t ii = 0; ii < 4; ++ii) {
                if (psg[ii].lchan) {
                    l -= psg[ii].channelA >> 1;
                    l -= psg[ii].channelB >> 1;
                    l -= psg[ii].channelC >> 1;
                }
                if (psg[ii].rchan) {
                    r -= psg[ii].channelA >> 1;
                    r -= psg[ii].channelB >> 1;
                    r -= psg[ii].channelC >> 1;
                }
            }
            break;

        default:    // mono, all channels go through both sides.
            l -= psg[0].channelA + psg[0].channelB + psg[0].channelC;
            r -= psg[0].channelA + psg[0].channelB + psg[0].channelC;
            break;
    }

    SoundChannel::getChannel().push(l, r);
}

int Spectrum::dac(size_t c) {

    return filter[c].get();
}

void Spectrum::setPage(uint_fast8_t page,
        uint_fast8_t bank, bool isRom, bool isContended) {

    size_t addr = bank * (1 << 14);
    mem[page] = (isRom) ? &rom[addr] : &ram[addr];
    romPage[page] = isRom;
    contendedPage[page] = isContended;
}

void Spectrum::setScreenPage(uint_fast8_t page) {

    scr = &ram[page * (1 << 14)];
}

void Spectrum::setSnowPage(uint_fast8_t page) {

    sno = &ram[page * (1 << 14)];
}

void Spectrum::checkTapeTraps() {

    if (rom48 && (z80.state == Z80State::ST_OCF_T4L_RFSH2)) {
        switch (z80.pc.w) {
            // These addresses are catched on the REFRESH cycles. Since PC
            // has been already incremented, these values are the intended
            // address plus one.
            case 0x056D:    // LD_START (0x56C + 1)
                // Note: Trap at 0x056D so the break check is performed.
                // Otherwise, the emulator will hang if the searched block
                // is not found.
                if (tape.tapData.size()) {
                    trapLdStart();
                }
                break;
            case 0x04D1:    // SA_FLAG (0x04D0 + 1)
                trapSaBytes();
                break;
            default:
                break;
        }
    }
}

void Spectrum::writeMemory(uint_fast16_t a, uint_fast8_t d) {

    a &= 0xFFFF;
    if (a > 0x3FFF) { // Don't write ROM.
        mem[a >> 14][a & 0x3FFF] = d;
    }
}

uint_fast8_t Spectrum::readMemory(uint_fast16_t a) {

    a &= 0xFFFF;
    return mem[a >> 14][a & 0x3FFF];
}

void Spectrum::trapLdStart() {

    // ZF = 1 means treat the flag byte as a normal byte. This is
    // indicated by setting the number of flag bytes to zero.
    uint16_t flagByte = (z80.af_.b.l & FLAG_Z) ? 1 : 0;

    // If either there are no flag bytes or the expected flag matches the
    // block's flag, we signal flag ok. Expected flag is in A'.
    bool flagOk = tape.foundTapBlock(z80.af_.b.h) || flagByte;

    // CF = 1 means LOAD, CF = 0 means VERIFY.
    bool verify = !(z80.af_.b.l & FLAG_C);

    if (flagOk) {
        // Get parameters from CPU registers
        uint16_t address = z80.ix.w;
        uint16_t bytes = z80.de.w;

        uint16_t block = tape.getBlockLength() + flagByte - 1;  // Include parity
        uint16_t offset = 3 - flagByte;
        z80.hl.b.h = flagByte ? 0 : tape.getBlockByte(2);

        if (verify) {
            while (bytes && block) {
                z80.hl.b.l = tape.getBlockByte(offset++);
                uint8_t mem = readMemory(address++);
                block--;
                bytes--;
                z80.hl.b.h ^= z80.hl.b.l;
                if (z80.hl.b.l != mem) break;
            }
        } else {
            while (bytes && block) {
                z80.hl.b.l = tape.getBlockByte(offset++);
                writeMemory(address++, z80.hl.b.l);
                block--;
                bytes--;
                z80.hl.b.h ^= z80.hl.b.l;
            }
        }

        if (block) {
            z80.hl.b.l = tape.getBlockByte(offset);
            z80.hl.b.h ^= z80.hl.b.l;
        }

        if (!bytes && block && !z80.hl.b.h) {
            z80.af.b.l |= FLAG_C;
        } else {
            z80.af.b.l &= ~FLAG_C;
            if (!block) z80.af.b.l |= FLAG_Z;
        }

        z80.af.b.h = z80.hl.b.h;
        z80.ix.w = address;
        z80.de.w = bytes;
        z80.bc.b.h = 0xB0;  // B is set at $5D3
        z80.bc.b.l ^= 0x3;  // C is set at $569 as the initial EAR bit, red border.
                            // If the number of pulses is even, C value is just C ^ 3.
    }

    // Advance tape
    tape.nextTapBlock();

    // Force RET
    z80.decode(0xC9);
    z80.startInstruction();

    if (tape.tapPointer == 0) {
        tape.rewind();
    }
}

void Spectrum::trapSaBytes() {

    uint16_t start = z80.ix.w;
    uint16_t bytes = z80.de.w + 2;
    uint8_t checksum;

    tape.saveData.push_back(bytes & 0x00FF);
    tape.saveData.push_back((bytes & 0xFF00) >> 8);
    tape.saveData.push_back(z80.af.b.h);
    bytes -= 2;

    z80.ix.w = (z80.ix.w + bytes) & 0xFFFF;
    z80.de.w -= bytes;

    checksum = z80.af.b.h;
    for (uint_fast16_t ii = 0; ii < bytes; ++ii) {
        uint_fast8_t byte = readMemory(start + ii);
        tape.saveData.push_back(byte);
        checksum ^= byte;
    }

    tape.saveData.push_back(checksum);

    // Force RET
    z80.decode(0xC9);
    z80.startInstruction();
}

void Spectrum::setSoundRate(SoundRate rate, bool syncToVideo) {

    switch (rate) {
        case SoundRate::SOUNDRATE_128K:
            clockRate = BASE_CLOCK_128;
            frame = FRAME_TIME_128;
            psgPeriod = static_cast<uint_fast32_t>(4e14 / static_cast<double>(BASE_CLOCK_128));
            break;
        case SoundRate::SOUNDRATE_PENTAGON:
            clockRate = BASE_CLOCK_48;
            frame = FRAME_TIME_PENTAGON;
            psgPeriod = static_cast<uint_fast32_t>(4e14 / static_cast<double>(BASE_CLOCK_48));
            break;
        default:
            clockRate = BASE_CLOCK_48;
            frame = FRAME_TIME_48;
            psgPeriod = static_cast<uint_fast32_t>(4e14 / static_cast<double>(BASE_CLOCK_48));
            break;
    }

    if (syncToVideo) {
        clockRate *= (static_cast<double>(frame) / static_cast<double>(FRAME_TIME_50HZ));
    }
}

bool Spectrum::allowPageChange() {

    switch (ula.ulaVersion) {
        case 5: // Pentagon: Allow page change always (?)
            return true;
        case 4: // +2A/+3: Allow page change when the ULA is not reading.
            return ula.mem;
        case 3: // 128/+2: Allow page change when no accesses are in progress.
        case 2: // fall-through
            return ula.mem && ((z80.c & SIGNAL_IORQ_) == SIGNAL_IORQ_);
        default: // 48K models: Forbid all page changes.
            return false;
    }
}

void Spectrum::loadState(SaveState const& state) {

    switch (state.model) {
        case SnapshotModel::ZX_48K_ISSUE2:
            setIssue2(RomVariant::ROM_48_EN);
            if (state.emuAy8912) psgChips = 1;
            if (state.emuFuller) joystick = JoystickType::FULLER;
            if (state.emuFuller || state.emuAy8912) psgPlaySound(true);
            break;
        case SnapshotModel::ZX_48K_ISSUE3:
            setIssue3(RomVariant::ROM_48_EN);
            if (state.emuAy8912) psgChips = 1;
            if (state.emuFuller) joystick = JoystickType::FULLER;
            if (state.emuFuller || state.emuAy8912) psgPlaySound(true);
            break;
        case SnapshotModel::ZX_128K:
            set128K(RomVariant::ROM_128_EN);
            pageRegs = state.port_0x7ffd;
            updatePage();
            break;
        case SnapshotModel::ZX_PLUS2:
            setPlus2(RomVariant::ROM_PLUS2_EN);
            pageRegs = state.port_0x7ffd;
            updatePage();
            break;
        case SnapshotModel::ZX_PLUS2A:
            setPlus2A(RomVariant::ROM_PLUS3_EN);
            pageRegs = state.port_0x1ffd << 8 | state.port_0x7ffd;
            updatePage();
            break;
        case SnapshotModel::ZX_PLUS3:
            setPlus3(RomVariant::ROM_PLUS3_EN); break;
            pageRegs = state.port_0x1ffd << 8 | state.port_0x7ffd;
            updatePage();
            break;
        case SnapshotModel::PENTAGON:
            setPentagon(RomVariant::ROM_PENTAGON);
            pageRegs = state.port_0x7ffd;
            updatePage();
            break;
        default:
            cout << "Error: Snapshot requests an unsupported ZX Spectrum model." << endl;
            return;
    }

    z80.sp.w = state.sp;
    z80.ix.w = state.ix;
    z80.iy.w = state.iy;

    z80.af.w = state.af;
    z80.bc.w = state.bc;
    z80.de.w = state.de;
    z80.hl.w = state.hl;

    z80.af_.w = state.af_;
    z80.bc_.w = state.bc_;
    z80.de_.w = state.de_;
    z80.hl_.w = state.hl_;

    z80.iff = state.iff;
    z80.im = state.im;

    if (state.emuRefresh) {
        z80.ir.w = state.ir;
    } else {
        z80.ir.b.h = (state.ir >> 8);
    }

    ula.borderAttr = state.border;

    for (size_t ii = 0; ii < 8; ++ii) {
        if (state.memory[ii].size()) {
            copy(state.memory[ii].begin(), state.memory[ii].end(), &ram[0x4000 * ii]);
        }
    }

    ula.start();

    if (state.type == SnapType::SNA_48) {
        z80.prefix = PREFIX_ED;
        z80.decode(0x45);
        z80.startInstruction();
        z80.state = Z80State::ST_OCF_T4L_RFSH2;

        while (ula.scan != ula.vSyncStart || ula.pixel != ula.interruptEnd) {
            ula.clock();
        }
    } else if (state.type == SnapType::SNA_128) {
        while (ula.scan != ula.vSyncStart || ula.pixel != ula.interruptEnd) {
            ula.clock();
        }

        z80.pc.w = state.pc;
        z80.state = Z80State::ST_OCF_T1H_ADDRWR;
    } else {
        uint32_t tStatesHi = (state.tStates / 0x10000) % 4;
        uint32_t tStatesLo = state.tStates % 0x10000;
        uint32_t cyclesPerArea = (ula.maxScan * ula.checkPointValues[ula.ulaVersion][5]) / 8;

        // First, advance the ULA until "just after the ULA generates its once-in-every-20-ms interrupt".
        while (ula.scan != ula.vSyncStart || ula.pixel != ula.interruptEnd) {
            ula.clock();
        }

        // If tStatesLo has a valid value (not higher than the number to total T-states divided by 4)...
        if (tStatesLo <= cyclesPerArea) {
            // We have to advance the ULA this number of cycles
            uint32_t cycles = cyclesPerArea * ((tStatesHi + 1) % 4) + 2 * (cyclesPerArea - tStatesLo);
            for (uint32_t ii = 0; ii < cycles; ++ii) {
                ula.clock();
            }
        }

        if (state.emuFuller) {
            for (size_t ii = 0; ii < 16; ++ii) {
                psg[4].r[ii] = state.ayRegs[ii];
            }
        } else {
            joystick = state.joystick;
        }

        if (state.emuAy8912) {
            for (size_t ii = 0; ii < 16; ++ii) {
                psg[0].r[ii] = state.ayRegs[ii];
            }
        }

        z80.pc.w = state.pc;
        z80.state = Z80State::ST_OCF_T1H_ADDRWR;
    }
}
// vim: et:sw=4:ts=4
