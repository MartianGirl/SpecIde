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

#include "CPC.h"

#include <cstdlib>
#include <ctime>

CPC::CPC() : romPage{true, false, false, true} {

    // This is just for the laughs. We initialize the whole RAM to the
    // values that appeared in the Spectrum at boot time.
    for (size_t ii = 0; ii < (1 << 16); ii += 2) {
        *(reinterpret_cast<uint32_t*>(ram) + ii) = 0x00000000;
        *(reinterpret_cast<uint32_t*>(ram) + ii + 1) = 0xFFFFFFFF;
    }

    setPage(0, 0, true);
    setPage(1, 1, false);
    setPage(2, 2, false);
    setPage(3, 1, true);

    channel.open(2, SAMPLE_RATE);
}

void CPC::loadRoms(RomVariant model) {

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
        case RomVariant::ROM_CPC464:
        romNames.push_back("cpc464-0.rom");
        romNames.push_back("cpc464-1.rom");
        break;

        case RomVariant::ROM_CPC664:
        romNames.push_back("cpc664-0.rom");
        romNames.push_back("cpc664-1.rom");
        romNames.push_back("amsdos.rom");
        break;

        case RomVariant::ROM_CPC6128:
        romNames.push_back("cpc6128-0.rom");
        romNames.push_back("cpc6128-1.rom");
        romNames.push_back("amsdos.rom");
        break;

        default:
        romNames.push_back("cpc464-0.rom");
        romNames.push_back("cpc464-1.rom");
        break;
    }

    size_t j = 0;
    bool fail = true;

    do {
        for (size_t i = 0; i < romNames.size(); ++i) {
            romName = romPaths[j] + romNames[i];
            cout << "Trying ROM: " << romName << endl;
            ifs.open(romName, ifstream::binary);

            // If it fails, try the ROM in /usr/share/spectrum-roms
            fail = ifs.fail();
            if (fail) {
                break;
            }

            char c;
            while (ifs.get(c)) {
                rom[((2 << 14) * i) + pos++] = c;
            }

            ifs.close();
        }
        ++j;
    } while (fail && j < romPaths.size());
}

void CPC::set464() {

    cpc128K = false;
    cpcDisk = false;

    loadRoms(RomVariant::ROM_CPC464);
    reset();
}

void CPC::set664() {

    cpc128K = false;
    cpcDisk = true;

    loadRoms(RomVariant::ROM_CPC664);
    reset();
}

void CPC::set6128() {

    cpc128K = true;
    cpcDisk = true;

    loadRoms(RomVariant::ROM_CPC6128);
    reset();
}

void CPC::run() {
}

void CPC::clock() {

    bool as_ = z80.c & SIGNAL_MREQ_;
    bool io_ = z80.c & SIGNAL_IORQ_;

    size_t memArea = z80.a >> 14;

    // First we clock the Gate Array. Further clocks will be generated here.
    ga.clock();

    if (ga.cClkEdge) {
        ctrc.clock();
    }

    ++count;
    if (!(count & 0x03)) {
        psgClock();
        if (cpcDisk && !(count & 0x07))
            fdc.clock();
    }

    // Z80 gets data from the ULA or memory, only when reading.
    if (!io_) {
        // RAM pagination
        if ((z80.a & 0x8000) == 0x0000) {

        }

        // Gate Array
        if ((z80.a & 0xC000) == 0x4000) {
            if (wr) {
                ga.write(z80.d);
            }
        }

        // CRTC
        if ((z80.a & 0x4000) == 0x0000) {
            switch (z80.a & 0x0300) {
                case 0x0000:    // CRTC Register Select (WO) at &BC00
                    crtc.wrAddress(z80.d);
                    break;
                case 0x0100:    // CRTC Register Write (WO) at &BD00
                    crtc.wrRegister(z80.d);
                    break;
                case 0x0200:    // CRTC Status Register Read (only type 1)
                    z80.d = crtc.rdStatus();
                    break;
                case 0x0300:    // CRTC Register Read (RO)
                    z80.d = crtc.rdRegister();
                    break;
                default:
                    break;
            }
        }

        // PPI
        if ((z80.a & 0x0800) == 0x0000) {
            switch (z80.a & 0x0300) {
                case 0x0000:    // Port A
                    break;
                case 0x0100:    // Port B
                    break;
                case 0x0200:    // Port C
                    break;
                case 0x0300:    // Control register
                    break;
                default:
                    break;
            }
        }

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

        // AY-3-8912 ports.
        {
            switch (z80.a & 0xC002)
            {
                case 0x8000:    // 0xBFFD
                    if (wr)
                        psgWrite();
                    else if (rd && spectrumPlus2A)
                        psgRead();
                    break;

                case 0xC000:    // 0xFFFD
                    if (wr)
                    {
                        if ((z80.d & 0x98) == 0x98)
                        {
                            currentPsg = (~z80.d) & 0x07;
                            psg[currentPsg].lchan = (z80.d & 0x40);
                            psg[currentPsg].rchan = (z80.d & 0x20);
                        }
                        else
                            psgAddr();
                    }
                    else if (rd)
                    {
                        psgRead();
                    }
                    break;

                default:
                    break;
            }
        }
    } else if (!as_) {
        if (rd) {
            z80.d = map[memArea][z80.a & 0x3FFF];
        } else if (!romPage[memArea] && wr) {
            map[memArea][z80.a & 0x3FFF] = z80.d;
        }
    }
    z80.clock();
}

void CPC::reset() {

    z80.reset();
    psgReset();
    fdc.reset();
}

void CPC::psgRead() {

    if (currentPsg < psgChips) {
        z80.d = psg[currentPsg].read();
    }
}

void CPC::psgWrite() {

    if (currentPsg < psgChips) {
        psg[currentPsg].write(z80.d);
    }
}

void CPC::psgAddr() {

    if (currentPsg < psgChips) {
        psg[currentPsg].addr(z80.d);
    }
}

void CPC::psgReset() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].reset();
        psg[ii].seed = 0xFFFF - (ii * 0x1111);
    }
}

void CPC::psgClock() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].clock();
    }
}

void CPC::psgPlaySound(bool play) {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].playSound = play;
    }
}

void CPC::psgSample() {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].sample();
    }
}

void CPC::psgChip(bool aychip) {

    for (size_t ii = 0; ii < psgChips; ++ii) {
        psg[ii].setVolumeLevels(aychip);
    }
}

void CPC::sample(int& l, int& r) {

    buzzer.sample();
    psgSample();
    l = r = buzzer.signal;

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

void CPC::setPage(uint_fast8_t page, uint_fast8_t bank, bool isRom) {

    size_t addr = bank * (2 << 14);
    map[page] = (isRom) ? &rom[addr] : &ram[addr];
    romPage[page] = isRom;
}
// vim: et:sw=4:ts=4
