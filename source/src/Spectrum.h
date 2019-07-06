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

#pragma once

/** Spectrum
 *
 * A Spectrum computer
 *
 */

#include "Memory.h"
#include "ULA.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "PSG.h"
#include "FDC.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

enum class StereoMode
{
    STEREO_MONO,
    STEREO_ABC,
    STEREO_ACB,
    STEREO_TURBO_MONO,
    STEREO_TURBO_ABC,
    STEREO_TURBO_ACB,
    STEREO_NEXT
};

class Spectrum
{
    public:
        Spectrum();

        // Required hardware.
        Z80 z80;
        ULA ula;
        PSG psg[4];
        FDC fdc;

        uint_fast8_t bus;
        uint_fast8_t bus_1;

        // Kempston Joystick.
        uint_fast8_t joystick;
        bool kempston;
        bool spectrum128K;
        bool spectrumPlus2;
        bool spectrumPlus2A;
        bool spectrumPlus3;
        bool psgPresent[8];
        size_t currentPsg;
        uint_fast8_t idle;
        uint_fast16_t paging;
        uint_fast16_t mask;

        int l = 0;
        int r = 0;

        bool contendedPage[4];
        bool romPage[4];

        uint_fast8_t ram[2 << 17];
        uint_fast8_t rom[2 << 16];
        uint_fast8_t* map[4];
        uint_fast8_t* scr;
        uint_fast8_t* sno;

        bool set48;
        bool rom48;

        size_t count = 0;

        StereoMode stereo;

        // This one is going to be called at 7MHz, and is going to:
        // 1. Clock the ULA. This starts the ULA counters.
        // 2. Access memory for the ULA, if the ULA is not high impedance.
        // 3. Clock the Z80, if the ULA says so.
        // 4. Access the memory for the Z80.
        void clock();
        void reset();

        void loadRoms(size_t model);
        void initMems(size_t model);
        void set128K();
        void setPlus2();
        void setPlus2A();
        void setPlus3();
        void updatePage(uint_fast8_t reg);
        void setPage(uint_fast8_t page,
                uint_fast8_t bank, bool isRom, bool isContended);
        void setScreenPage(uint_fast8_t page);
        void setSnowPage(uint_fast8_t page);

        void psgSelect();
        void psgRead();
        void psgWrite();
        void psgAddr();
        void psgReset();
        void psgClock();
        void psgSample();
        void psgChip(bool play);
        void psgPlaySound(bool play);

        void sample();
};

// vim: et:sw=4:ts=4
