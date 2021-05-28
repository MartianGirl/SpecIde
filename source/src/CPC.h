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

#include "GateArray.h"
#include "PPI.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "PSG.h"
#include "FDC765.h"
#include "Tape.h"
#include "config.h"

#include "CommonDefs.h"
#include "SoundDefs.h"
#include "SoundChannel.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

struct ExpansionRom {

    std::string name;
    std::vector<uint8_t> data;

    ExpansionRom() : name(), data(0x4000, 0x00) {}
    ExpansionRom(std::string n) : name(n), data(0x4000, 0x00) {}
};

/**
 * CPC
 *
 * An Amstrad CPC computer.
 *
 * This class represents the following Amstrad CPC models.
 */
class CPC {

    public:
        /**
         * Constructor.
         */
        CPC();

        /** Z80 CPU instance. */
        Z80 z80;
        /** Gate Array instance. */
        GateArray ga;
        /** 8255 PPI instance. */
        PPI ppi;
        /** PSG instance. */
        PSG psg;
        /** FDC 765 instance. */
        FDC765 fdc765;
        /** Tape drive. */
        Tape tape;
        /** Keyboard matrix. */
        uint_fast8_t keys[16];

        /** Sound channel object. */
        SoundChannel channel;

        uint_fast8_t bus = 0xFF;

        uint_fast8_t joystick = 0x00;
        uint_fast16_t pageRegs = 0x00;

        bool cpc128K = true;
        bool cpcDisk = true;
        bool expBit = false;

        bool tapeSound = false;
        bool playSound = false;
        StereoMode stereo = StereoMode::STEREO_MONO;



        /** RAM array. RAM pages are defined as pointers in this array. */
        uint8_t ram[1 << 17];
        /** Internal ROM array. */
        uint8_t rom[1 << 15];
        /** Currently selected RAM pages. */
        uint8_t* mem[4];
        /** External ROM map. ROM pages are defined as pointers in this array. */
        std::map<uint8_t, ExpansionRom> ext;
        /** Installed Expansion ROMs. */
        bool extReady[0x100];
        /** Currently selected expansion ROM page. */
        uint_fast8_t romBank;
        /** Pointer to lower ROM. */
        uint8_t* loRom = &rom[0x0000];
        /** Pointer to high ROM. */
        uint8_t* hiRom = &rom[0x4000];

        size_t counter = 0;
        bool updateMotor = false;

        void run();

        // This one is going to be called at 8MHz, and is going to:
        // 1. Clock the GA. This starts the GA counters.
        // 2. Access memory for the GA, if the GA is not high impedance.
        // 3. Clock the Z80, if the GA says so.
        // 4. Access the memory for the Z80.
        void clock();
        void reset();

        void scanKeys();
        /**
         * Load internal ROM for each Amstrad CPC model.
         *
         * @param model The Amstrad CPC model.
         */
        void loadRoms(RomVariant model);

        /**
         * Load expansion ROMs.
         */
        void loadExpansionRoms();

        /**
         * Configure an Amstrad CPC 464 computer.
         */
        void set464();

        /**
         * Configure an Amstrad CPC 664 computer.
         */
        void set664();

        /**
         * Configure an Amstrad CPC 6128 computer.
         */
        void set6128();

        /**
         * Update RAM mapping.
         *
         * @param byte Z80 byte written to selection hardware (PAL 40030)
         */
        void selectRam(uint_fast8_t reg);

        /**
         * Update RAM page entry in the memory map.
         *
         * @param page Memory map RAM page:
         *  <ul>
         *      <li>0: $0000-$3FFF.</li>
         *      <li>1: $4000-$7FFF.</li>
         *      <li>2: $8000-$BFFF.</li>
         *      <li>3: $C000-$FFFF.</li>
         *  </ul>
         * @param bank RAM bank to use.
         */
        void setPage(uint_fast8_t page, uint_fast8_t bank);

        void psgReset();
        void psgSample();
        void psgChip(bool play);
        void psgPlaySound(bool play);

        void sample(int& l, int& r);
};

// vim: et:sw=4:ts=4
