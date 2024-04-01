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

uint_fast8_t constexpr BRAND_ISP = 0x00;
uint_fast8_t constexpr BRAND_TRIUMPH = 0x01;
uint_fast8_t constexpr BRAND_SAISHO = 0x02;
uint_fast8_t constexpr BRAND_SOLAVOX = 0x03;
uint_fast8_t constexpr BRAND_AWA = 0x04;
uint_fast8_t constexpr BRAND_SCHNEIDER = 0x05;
uint_fast8_t constexpr BRAND_ORION = 0x06;
uint_fast8_t constexpr BRAND_AMSTRAD = 0x07;

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
        /**
         * Destructor.
         */
        ~CPC() { channel.close(); }

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
        uint_fast8_t keys[10];

        /** Sound channel object. */
        SoundChannel channel;

        uint_fast8_t bus = 0xFF;

        uint_fast16_t pageRegs = 0x00;

        bool cpc128K = true;
        bool cpcDisk = true;
        bool expBit = false;

        bool tapeSound = false;

        /** Tape signal level. */
        uint_fast8_t tapeLevel = 0;
        /** Tape relay status. */
        bool relay = false;
        /** Tape speed counter. */
        uint_fast32_t tapeSpeed = 0;
        /** Gate Array cycle counter. */
        uint_fast32_t cycles = 0;

        /** Sample array for tape sound. */
        int filter[FILTER_BZZ_SIZE];
        /** Index variable for filter. */
        size_t index = 0;
        /** Number of cycles before next sound sample. */
        uint_fast32_t skip;
        /** Tail of cycles before next sound sample. */
        uint_fast32_t tail;
        /** Counter of cycles before next sound sample. */
        uint_fast32_t skipCycles = 0;

        uint_fast8_t brand = BRAND_AMSTRAD;

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

        bool updateMotor = false;

        /**
         * Run one frame of emulation, or the default frame time.
         *
         * @param frame Run for a complete frame, no matter the time.
         */
        void run(bool frame);


        void generateSound();

        // This one is going to be called at 8MHz, and is going to:
        // 1. Clock the GA. This starts the GA counters.
        // 2. Access memory for the GA, if the GA is not high impedance.
        // 3. Clock the Z80, if the GA says so.
        // 4. Access the memory for the Z80.
        void clock();
        void reset();

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
         *
         * @param model The Amstrad CPC model.
         */
        void set464(RomVariant model);

        /**
         * Configure an Amstrad CPC 664 computer.
         *
         * @param model The Amstrad CPC model.
         */
        void set664(RomVariant model);

        /**
         * Configure an Amstrad CPC 6128 computer.
         *
         * @param model The Amstrad CPC model.
         */
        void set6128(RomVariant model);

        /**
         * Configure model brand.
         */
        void setBrand(uint_fast8_t brand);

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

        /**
         * Reset the PSG.
         */
        void psgReset();

        /**
         * Select PSG type.
         *
         * @param aychip PSG type. (false = AY-8912-3, true = YM-2149)
         */
        void psgChip(bool play);

        /**
         * Mute or unmute the PSG.
         */
        void psgPlaySound(bool play);

        /**
         * Mix and sample sound from all sources (Tape, PSG)
         */
        void sample();

        void setSoundRate(uint_fast32_t frame, bool syncToVideo);

        /**
         * Toggle sound playback.
         *
         * @param play Enable sound.
         */
        void playSound(bool play);
};

// vim: et:sw=4:ts=4
