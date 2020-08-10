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

/** CPC
 *
 * An Amstrad CPC computer
 *
 */

#include "Memory.h"
#include "CRTC.h"
#include "GateArray.h"
#include "PPI.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "PSG.h"
#include "FDC.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class CPC
{
    public:
        CPC();

        // Required hardware.
        Z80 z80;
        CRTC crtc;
        GateArray ga;
        PPI ppi;
        PSG psg;
        FDC fdc;

        uint_fast8_t bus;

        uint_fast8_t joystick;
        uint_fast16_t paging;

        bool contendedPage[4];
        bool romPage[4];

        uint_fast8_t ram[2 << 17];
        uint_fast8_t rom[2 << 16];
        uint_fast8_t* map[4];
        uint_fast8_t* scr;

        size_t count = 0;

        // This one is going to be called at 8MHz, and is going to:
        // 1. Clock the GA. This starts the GA counters.
        // 2. Access memory for the GA, if the GA is not high impedance.
        // 3. Clock the Z80, if the GA says so.
        // 4. Access the memory for the Z80.
        void clock();
        void reset();

        void loadRoms(size_t model);
        void initMems(size_t model);
        void set464();
        void set664();
        void set6128();
        void updatePage(uint_fast8_t reg);
        void setPage(
                uint_fast8_t page, uint_fast8_t bank,
                bool isRom, bool isContended)
        {
            size_t addr = bank * (2 << 14);
            map[page] = (isRom) ? &rom[addr] : &ram[addr];
            romPage[page] = isRom;
            contendedPage[page] = isContended;
        }

        void setScreen(uint_fast8_t page)
        {
            scr = &ram[page * (2 << 14)];
        }

        void psgRead();
        void psgWrite();
        void psgAddr();
        void psgReset();
        void psgClock();
        void psgSample();
        void psgChip(bool play);
        void psgPlaySound(bool play);

        void sample(int& l, int& r);
};

// vim: et:sw=4:ts=4
