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

CPC::CPC() :
    bus(0xFF),
    joystick(0),
    idle(0xFF),
{
    // This is just for the laughs. We initialize the whole RAM to random
    // values to see the random attributes that appeared in the Spectrum
    // at boot time.
    srand(static_cast<unsigned int>(time(0)));
    for (size_t a = 0; a < (2 << 17); ++a)
        ram[a] = rand() & 0xFF;
}

void CPC::loadRoms(size_t model)
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


void CPC::set464()
{
    cpc128K = false;
    cpcDisk = false;
    reset();
}

void CPC::set664()
{
    cpc128K = false;
    cpcDisk = true;
    reset();
}

void CPC::set6128()
{
    cpc128K = true;
    cpcDisk = true;
    reset();
}

void CPC::clock()
{
    bool as_ = z80.c & SIGNAL_MREQ_;
    bool io_ = z80.c & SIGNAL_IORQ_;
    bool rd = z80.access && !(z80.c & SIGNAL_RD_);
    bool wr = z80.access && !(z80.c & SIGNAL_WR_);

    size_t memArea = z80.a >> 14;

    // First we clock the Gate Array. Further clocks will be generated here.
    ga.clock();

    if (ga.cClkEdge) {
        ctrc.clock();
    }

    ++count;
    if (!(count & 0x03))
    {
        psgClock();

        if (cpcDisk && !(count & 0x07))
            fdc.clock();
    }

    {
        // Z80 gets data from the ULA or memory, only when reading.
        if (!io_)
        {
            // RAM pagination
            if ((z80.a & 0x8000) == 0x0000)
            {

            }

            // Gate Array
            if ((z80.a & 0xC000) == 0x4000)
            {
                if (wr)
                    ga.write(z80.d);
            }

            // CRTC
            if ((z80.a & 0x4000) == 0x0000)
            {
                switch (z80.a & 0x0300)
                {
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
            if ((z80.a & 0x0800) == 0x0000)
            {
                switch (z80.a & 0x0300)
                {
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
            if ((z80.a & 0x0400) == 0x0000)
            {
                // FDC
                if ((z80.a & 0x0080) == 0x0000)
                {
                    switch (z80.a & 0x0101)
                    {
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
        }
        else if (!as_)
        {
            // Bank 0: 0000h - ROM
            // Bank 1: 4000h - Contended memory
            // Bank 2: 8000h - Extended memory
            // Bank 3: C000h - Extended memory (can be contended)
            if (rd)
            {
                z80.d = map[memArea][z80.a & 0x3FFF];
            }
            else if (!romPage[memArea] && wr)
            {
                map[memArea][z80.a & 0x3FFF] = z80.d;
            }
        }

        z80.clock();
    }
}


void CPC::reset()
{
    z80.reset();
    psgReset();
    fdc.reset();

}

void CPC::psgRead()
{
    if (psgPresent[currentPsg])
        z80.d = psg[currentPsg].read();
}

void CPC::psgWrite()
{
    if (psgPresent[currentPsg])
        psg[currentPsg].write(z80.d);
}

void CPC::psgAddr()
{
    if (psgPresent[currentPsg])
        psg[currentPsg].addr(z80.d);
}

void CPC::psgReset()
{
    psg[0].reset();
    psg[1].reset();
    psg[2].reset();
    psg[3].reset();
    psg[4].reset();
    psg[5].reset();
    psg[6].reset();
    psg[7].reset();
}

void Spectrum::psgClock()
{
    if (psgPresent[0]) psg[0].clock();
    if (psgPresent[1]) psg[1].clock();
    if (psgPresent[2]) psg[2].clock();
    if (psgPresent[3]) psg[3].clock();
    if (psgPresent[4]) psg[4].clock();
    if (psgPresent[5]) psg[5].clock();
    if (psgPresent[6]) psg[6].clock();
    if (psgPresent[7]) psg[7].clock();
}

void Spectrum::psgPlaySound(bool play)
{
    psg[0].playSound = play;
    psg[1].playSound = play;
    psg[2].playSound = play;
    psg[3].playSound = play;
    psg[4].playSound = play;
    psg[5].playSound = play;
    psg[6].playSound = play;
    psg[7].playSound = play;
}

void Spectrum::psgSample()
{
    if (psgPresent[0]) psg[0].sample();
    if (psgPresent[1]) psg[1].sample();
    if (psgPresent[2]) psg[2].sample();
    if (psgPresent[3]) psg[3].sample();
    if (psgPresent[4]) psg[4].sample();
    if (psgPresent[5]) psg[5].sample();
    if (psgPresent[6]) psg[6].sample();
    if (psgPresent[7]) psg[7].sample();
}

void Spectrum::psgChip(bool aychip)
{
    psg[0].setVolumeLevels(aychip);
    psg[1].setVolumeLevels(aychip);
    psg[2].setVolumeLevels(aychip);
    psg[3].setVolumeLevels(aychip);
    psg[4].setVolumeLevels(aychip);
    psg[5].setVolumeLevels(aychip);
    psg[6].setVolumeLevels(aychip);
    psg[7].setVolumeLevels(aychip);
}

void Spectrum::sample(int& l, int& r)
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
            for (size_t ii = 0; ii < 8; ++ii)
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

// vim: et:sw=4:ts=4
