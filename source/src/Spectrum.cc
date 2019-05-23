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
    kempston(false),
    spectrum128K(false),
    spectrumPlus2(false),
    spectrumPlus2A(false),
    spectrumPlus3(false),
    psgPresent{false, false, false, false},
    currentPsg(0),
    idle(0xFF),
    paging(0x0020), mask(0x0001),
    contendedPage{false, true, false, false},
    romPage{true, false, false, false},
    set48(true), rom48(true),
    stereo(StereoMode::STEREO_MONO)
{
    buzzer.init(&ula.soundBits, &ula.tapeIn);

    // This is just for the laughs. We initialize the whole RAM to random
    // values to see the random attributes that appeared in the Spectrum
    // at boot time.
    srand(static_cast<unsigned int>(time(0)));
    for (size_t a = 0; a < (2 << 17); ++a)
        ram[a] = rand() & 0xFF;

    setPage(0, 0, true, false);
    setPage(1, 5, false, true);
    setPage(2, 2, false, false);
    setPage(3, 0, false, false);
    setScreen(5);
}

void Spectrum::loadRoms(size_t model)
{
    ifstream ifs;
    vector<string> romNames;
    vector<string> romPaths;
    string romName;

    char* pHome = getenv(SPECIDE_HOME_ENV);

    romPaths.push_back("");
    if (pHome != nullptr)
    {
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

    switch (model)
    {
        case 0:
            romNames.push_back("48.rom");
            break;

        case 1:
            romNames.push_back("128-0.rom");
            romNames.push_back("128-1.rom");
            break;

        case 2:
            romNames.push_back("plus2-0.rom");
            romNames.push_back("plus2-1.rom");
            break;

        case 3:
            romNames.push_back("plus3-0.rom");
            romNames.push_back("plus3-1.rom");
            romNames.push_back("plus3-2.rom");
            romNames.push_back("plus3-3.rom");
            break;

        case 4:
            romNames.push_back("128-spanish-0.rom");
            romNames.push_back("128-spanish-1.rom");
            break;

        case 5:
            romNames.push_back("plus2-spanish-0.rom");
            romNames.push_back("plus2-spanish-1.rom");
            break;

        case 6:
            romNames.push_back("plus3-spanish-0.rom");
            romNames.push_back("plus3-spanish-1.rom");
            romNames.push_back("plus3-spanish-2.rom");
            romNames.push_back("plus3-spanish-3.rom");
            break;

        default:
            romNames.push_back("48.rom");
            break;
    }

    size_t j = 0;
    bool fail = true;

    do
    {
        for (size_t i = 0; i < romNames.size(); ++i)
        {
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

void Spectrum::set128K()
{
    spectrum128K = true;
    spectrumPlus2 = false;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    psgPresent[0] = true;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2()
{
    spectrum128K = true;
    spectrumPlus2 = true;
    spectrumPlus2A = false;
    spectrumPlus3 = false;
    psgPresent[0] = true;
    mask = 0x0001;
    reset();
}

void Spectrum::setPlus2A()
{
    spectrum128K = false;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = false;
    psgPresent[0] = true;
    mask = 0x0004;
    reset();
}

void Spectrum::setPlus3()
{
    spectrum128K = false;
    spectrumPlus2 = false;
    spectrumPlus2A = true;
    spectrumPlus3 = true;
    psgPresent[0] = true;
    mask = 0x0004;
    reset();
}

void Spectrum::clock()
{
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
    bool rd = z80.access && !(z80.c & SIGNAL_RD_);
    bool wr = z80.access && !(z80.c & SIGNAL_WR_);

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

    if (!ula.mem)
    {
        if (!spectrumPlus2A)
        {
            // Snow effect
            bus = (contendedPage[memArea] && ula.snow && !as_) ?
                map[memArea][(ula.a & 0x3f80) | (z80.a & 0x007f)] :
                scr[ula.a];
        }
        else
        {
            bus = scr[ula.a];
        }
    }
    else
    {
        if (!spectrumPlus2A)
            bus = z80.d;
        else if (contendedPage[memArea] && !as_
                && (!(z80.c & SIGNAL_RD_) || !(z80.c & SIGNAL_WR_)))
            bus = z80.d;
    }

    ula.d = bus;

    ula.clock();
    z80.c = ula.z80_c;

    ++count;
    if (!(count & 0x03))
    {
        buzzer.update();
        psgClock();

        if (spectrumPlus3 && !(count & 0x07))
            fdc.clock();
    }

    // We clock the Z80 if the ULA allows.
    if (ula.cpuClock)
    {
        // Z80 gets data from the ULA or memory, only when reading.
        if (!io_)
        {
            // 48K/128K/Plus2 floating bus. Return idle status by default,
            // or screen data, if the ULA is working.
            if (rd)
            {
                z80.d = (ula.idle) ? idle : bus & idle;
            }

            // 128K only ports (paging, disk)
            if (spectrum128K)
            {
                if (!(z80.a & 0x8002))
                {
                    if (wr || rd)
                        updatePage(0);
                }
            }
            else if (spectrumPlus2A)
            {
                switch (z80.a & 0xF002)
                {
                    case 0x0000:    // In +2A/+3 this is the floating bus port.
                        if (rd)
                        {
                            if (!(paging & 0x0020))
                                z80.d = (bus_1 & idle) | 0x01;
                        }
                        break;
                    case 0x1000:    // 0x1FFD (+3 Paging High)
                        if (wr)
                            updatePage(1);
                        break;
                    case 0x2000:    // 0x2FFD (+3 FDC Main Status)
                        if (spectrumPlus3)
                        {
                            if (rd)
                                z80.d = fdc.status();
                        }
                        break;
                    case 0x3000:    // 0x3FFD (+3 FDC Data)
                        if (spectrumPlus3)
                        {
                            if (wr)
                                fdc.write(z80.d);
                            else if (rd)
                                z80.d = fdc.read();
                        }
                        break;
                    case 0x4000: // fall-through
                    case 0x5000: // fall-through
                    case 0x6000: // fall-through
                    case 0x7000: // 0x7FFD (128K Paging / +3 Paging Low)
                        if (wr)
                            updatePage(0);
                        break;

                    default:
                        break;
                }
            }

            // AY-3-8912 ports.
            if (psgPresent[0])  // If there are PSGs, there is a PSG 0
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
                                psgSelect();
                            else
                                psgAddr();
                        }
                        else if (rd)
                            psgRead();
                        break;

                    default:
                        break;
                }
            }

            // Common ports.
            if (kempston && !(z80.a & 0x00E0))  // Kempston joystick.
            {
                if (rd)
                    z80.d = joystick;
            }
            else if (!(z80.a & 0x0001))
            {
                if (wr)
                    ula.ioWrite(z80.d);
                else if (rd)
                    z80.d = ula.ioRead();
            }
        }
        else if (!as_)
        {
            // Bank 0: 0000h - ROM
            // Bank 1: 4000h - Contended memory
            // Bank 2: 8000h - Extended memory
            // Bank 3: C000h - Extended memory (can be contended)
            if (rd)
                z80.d = map[memArea][z80.a & 0x3FFF];
            else if (!romPage[memArea] && wr)
                map[memArea][z80.a & 0x3FFF] = z80.d;
        }
        else
            z80.d = 0xFF;

        z80.clock();
    }
}

void Spectrum::updatePage(uint_fast8_t reg)
{
    if (!(paging & 0x0020))
    {
        if (reg == 1)
            paging = (z80.d << 8) | (paging & 0x00FF);
        else
            paging = z80.d | (paging & 0xFF00);

        // Update +3 disk drive(s) motor status.
        fdc.motor(spectrumPlus3 && (paging & 0x0800));

        // Select screen to display.
        setScreen(((paging & 0x0008) >> 2) | 0x05);

        if (paging & 0x0100)    // Special paging mode.
        {
            switch (paging & 0x0600)
            {
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
        }
        else                                // Normal paging mode.
        {
            size_t ramBank = paging & 0x0007;
            size_t romBank =
                ((paging & 0x0010) >> 4) | ((paging & 0x0400) >> 9);

            setPage(0, romBank, true, false);
            setPage(1, 5, false, true);
            setPage(2, 2, false, false);
            setPage(3, ramBank, false, ((paging & mask) == mask));

            rom48 = ((spectrumPlus2A && romBank == 3)
                    || (spectrum128K && romBank == 1));
            set48 = (paging & 0x0020);
        }
    }
}

void Spectrum::reset()
{
    ula.reset();    // Synchronize clock level.
    z80.reset();
    psgReset();
    fdc.reset();

    if (spectrum128K || spectrumPlus2A)
    {
        setPage(0, 0, true, false);
        setPage(1, 5, false, true);
        setPage(2, 2, false, false);
        setPage(3, 0, false, false);
        setScreen(5);
        paging = 0x0000;
        set48 = false;
        rom48 = false;
    }
    else
    {
        setPage(0, 0, true, false);
        setPage(1, 5, false, true);
        setPage(2, 2, false, false);
        setPage(3, 0, false, false);
        setScreen(5);
        paging = 0x0020;
        set48 = true;
        set48 = true;
    }
}

void Spectrum::psgSelect()
{
    size_t newPsg = (~z80.d) & 0x03;
    if (psgPresent[newPsg])
    {
        currentPsg = newPsg;
        psg[currentPsg].lchan = (z80.d & 0x40);
        psg[currentPsg].rchan = (z80.d & 0x20);
    }
}

void Spectrum::psgRead()
{
    if (psgPresent[currentPsg])
        z80.d = psg[currentPsg].read();
}

void Spectrum::psgWrite()
{
    if (psgPresent[currentPsg])
        psg[currentPsg].write(z80.d);
}

void Spectrum::psgAddr()
{
    if (psgPresent[currentPsg])
        psg[currentPsg].addr(z80.d);
}

void Spectrum::psgReset()
{
    psg[0].reset();
    psg[1].reset();
    psg[2].reset();
    psg[3].reset();
}

void Spectrum::psgClock()
{
    if (psgPresent[0]) psg[0].clock();
    if (psgPresent[1]) psg[1].clock();
    if (psgPresent[2]) psg[2].clock();
    if (psgPresent[3]) psg[3].clock();
}

void Spectrum::psgPlaySound(bool play)
{
    psg[0].playSound = play;
    psg[1].playSound = play;
    psg[2].playSound = play;
    psg[3].playSound = play;
}

void Spectrum::psgSample()
{
    if (psgPresent[0]) psg[0].sample();
    if (psgPresent[1]) psg[1].sample();
    if (psgPresent[2]) psg[2].sample();
    if (psgPresent[3]) psg[3].sample();
}

void Spectrum::psgChip(bool aychip)
{
    psg[0].setVolumeLevels(aychip);
    psg[1].setVolumeLevels(aychip);
    psg[2].setVolumeLevels(aychip);
    psg[3].setVolumeLevels(aychip);
}

void Spectrum::sample()
{
    buzzer.sample();
    psgSample();

    l = r = buzzer.signal;

    switch (stereo)
    {
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
            for (size_t ii = 0; ii < 4; ++ii)
            {
                if (psg[ii].lchan)
                {
                    l += psg[ii].channelA >> 1;
                    l += psg[ii].channelB >> 1;
                    l += psg[ii].channelC >> 1;
                }
                if (psg[ii].rchan)
                {
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

void Spectrum::setPage(uint_fast8_t page,
        uint_fast8_t bank, bool isRom, bool isContended)
{
    size_t addr = bank * (2 << 14);
    map[page] = (isRom) ? &rom[addr] : &ram[addr];
    romPage[page] = isRom;
    contendedPage[page] = isContended;
}

void Spectrum::setScreen(uint_fast8_t page)
{
    scr = &ram[page * (2 << 14)];
}

// vim: et:sw=4:ts=4
