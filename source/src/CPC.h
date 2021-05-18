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

#pragma once

#include "Memory.h"
#include "CRTC.h"
#include "GateArray.h"
#include "PPI.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "PSG.h"
#include "FDC765.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/**
 * CPC
 *
 * An Amstrad CPC computer.
 *
 * This class represents the following Amstrad CPC models.
 */
class CPC {

    public:
        /** Z80 CPU instance. */
        Z80 z80;
        /** CRTC instance. */
        CRTC crtc;
        /** Gate Array instance. */
        GateArray ga;
        /** 8255 PPI instance. */
        PPI ppi;
        /** PSG instances. */
        PSG psg[4];
        /** FDC 765 instance. */
        FDC fdc;

        uint_fast8_t bus = 0xFF;

        uint_fast8_t joystick = 0x00;
        uint_fast16_t pageRegs = 0x00;

        bool cpc128K = true;
        bool cpcDisk = true;
        /** Emulate so many PSGs. */
        size_t psgChips = 0;
        /** Currently selected PSG. */
        size_t currentPsg = 0;

        /**
         * Map of ROM pages. In the CPC family, ROM can be paged in the
         * $0000-$3FFF and $C000-$FFFF ranges.
         */
        bool romPage[4];
        /** RAM array. RAM pages are defined as pointers in this array. */
        uint_fast8_t ram[2 << 17];
        /** ROM array. ROM pages are defined as pointers in this array. */
        uint_fast8_t rom[2 << 16];
        /** Currently selected pages (RAM or ROM). */
        uint_fast8_t* map[4];

        size_t counter = 0;

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

        /**
         * Update page info in the memory map.
         *
         * @param page Memory map page:
         *  <ul>
         *      <li>0: $0000-$3FFF.</li>
         *      <li>1: $4000-$7FFF.</li>
         *      <li>2: $8000-$BFFF.</li>
         *      <li>3: $C000-$FFFF.</li>
         *  </ul>
         * @param bank Memory bank to use.
         * @param isRom Use a ROM page if true.
         */
        void setPage(uint_fast8_t page, uint_fast8_t bank, bool isRom);

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
