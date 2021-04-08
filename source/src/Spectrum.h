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

#include "ULA.h"
#include "Z80.h"
#include "Z80Defs.h"
#include "PSG.h"
#include "FDC765.h"
//#include "FD1793.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

enum class StereoMode {
    STEREO_MONO,
    STEREO_ABC,
    STEREO_ACB,
    STEREO_TURBO_MONO,
    STEREO_TURBO_ABC,
    STEREO_TURBO_ACB,
    STEREO_NEXT
};

enum class RomVariant {
    ROM_48_EN,
    ROM_48_ES,
    ROM_128_EN,
    ROM_128_ES,
    ROM_PLUS2_EN,
    ROM_PLUS2_ES,
    ROM_PLUS3_EN,
    ROM_PLUS3_ES,
    ROM_PENTAGON
};

/**
 * A ZX Spectrum computer.
 *
 * This class represents all the ZX Spectrum models. Supported models are:
 *  - ZX Spectrum 48K (issue 2)
 *  - ZX Spectrum 48K (issue 3)
 *  - ZX Spectrum + (issue 3, Spanish)
 *  - ZX Spectrum 128K (English, Spanish ROMs)
 *  - ZX Spectrum +2 (English, Spanish ROMs)
 *  - ZX Spectrum +3 (English, Spanish ROMs)
 *  - ZX Spectrum +2A (English, Spanish ROMs)
 *  - ZX Pentagon 128 (English)
 *
 * Some peripherals are supported:
 *  - Kempston joystick interface.
 *  - Sinclair joystick interface.
 *  - Up to 4 PSGs. (AY-8912-3 or YM-2149)
 *  - Monophonic Covox on port $FB.
 */
class Spectrum {

    public:
        /**
         * Constructor.
         */
        Spectrum();

        /** Z80 CPU instance. */
        Z80 z80;
        /** ULA. Supports Sinclair ULAs, Amstrad Gate Array, and Pentagon ULA. */
        ULA ula;
        /** PSG instances. (AY-8912-3, YM-2149) */
        PSG psg[4];
        /** ZX Spectrum +3 floppy disk controller. (NEC765 or compatible.) */
        FDC765 fdc765;
        /** BetaDisk 128 floppy disk controller. (WD1793 or compatible.) */
        //FD1793 fd1793;

        /** Byte in bus. Used in floating bus effects. */
        uint_fast8_t bus = 0xFF;
        /** Byte in bus, delayed 1 clock. */
        uint_fast8_t bus_1 = 0xFF;
        /** Byte in Kempston joystick port. */
        uint_fast8_t joystick = 0x00;

        /** Kempston interface present. If false, Sinclair joystick is emulated. */
        bool kempston = false;
        /** Emulate a 128K spectrum (128K, +2, +2A, +3, Pentagon). */
        bool spectrum128K = false;
        /** Emulate a Gate Array based Spectrum (+2A, +3). */
        bool spectrumPlus2A = false;
        /** Emulate +3 disk controller and disk drives. */
        bool plus3Disk = false;
        /** Emulate BetaDisk128 disk interface. */
        bool betaDisk128 = false;
        /** Emulate so many PSGs. */
        size_t psgChips = 0;
        /** Currently selected PSG. */
        size_t currentPsg = 0;
        /** Value of idle bus. */
        uint_fast8_t idle = 0xFF;
        /** Value of pagination registers. (15-8: $1FFD, 7-0: $7FFD) */
        uint_fast16_t pageRegs = 0x0020;
        /** Mask for deciding if a high-RAM page is contended. */
        uint_fast16_t mask = 0x0001;
        /** Currently selected RAM page on $C000-$FFFF. */
        uint_fast8_t ramBank = 0x00;
        /** Currently selected ROM page on $0000-$3FFF. */
        uint_fast8_t romBank = 0x00;

        /** Sound byte in left sound channel. (Beeper + AY + Covox) */
        int l = 0;
        /** Sound byte in right sound channel. (Beeper + AY + Covox) */
        int r = 0;
        /** Sound byte in Covox port. */
        int covox = 0;
        /** Array of samples sent to the Covox. */
        int filter[FILTER_BZZ_SIZE];
        /** Current index in filter array. */
        size_t index = 0;

        /**
         * Map of contended memory areas. Typically, $4000-$7FFF is contended,
         * and $C000-$FFFF might be, but +2A/+3's special pagination mode allows other
         * configurations.
         */
        bool contendedPage[4];
        /**
         * Map of ROM memory areas. Typically, $0000-$3FFF is ROM, but in +2A/+3
         * there may be RAM instead.
         */
        bool romPage[4];

        /** RAM array. RAM pages are defined as pointers in this array. */
        uint_fast8_t ram[2 << 17];
        /** ROM array. ROM pages are defined as pointers in this array. */
        uint_fast8_t rom[2 << 16];
        /** Currently selected pages (RAM or ROM). */
        uint_fast8_t* map[4];
        /** Currently selected screen page. */
        uint_fast8_t* scr;
        /** Memory page from which snow data is read. */
        uint_fast8_t* sno;

        /** True if 48K mode is active. This disables pagination. */
        bool set48 = true;
        /** True if 48K BASIC ROM is selected. Useful for trapping tape routines. */
        bool rom48 = true;

        /** Clock cycle counter. */
        size_t count = 0;

        /** Stereo mode: ABC, ACB, Turbosound modes. */
        StereoMode stereo = StereoMode::STEREO_MONO;

        /**
         * Clock the Spectrum. This happens at 7MHz.
         *
         * This function does the following:
         *  - Clock the ULA. This updates all the ULA counters and signals.
         *  - Access memory for the ULA, the ULA is not in high impedance.
         *  - Clock the rest of the chips (PSG, FDC).
         *  - Evaluate all the port accesses.
         *  - Clock the Z80, if the ULA allows it.
         *  - Evaluate the memory accesses.
         */
        void clock();

        /**
         * Reset the Spectrum.
         */
        void reset();

        /**
         * Load the correct ROMs for the selected model.
         *
         * @param RomVariant The model whose ROMs will be loaded.
         */
        void loadRoms(RomVariant model);

        /**
         * Select ZX Spectrum 128K "toastrack" timings and settings.
         */
        void set128K();

        /**
         * Select ZX Spectrum +2 (grey case) timings and settings.
         */
        void setPlus2();

        /**
         * Select ZX Spectrum +2A (black case) timings and settings.
         */
        void setPlus2A();

        /**
         * Select ZX Spectrum +3 timings and settings.
         */
        void setPlus3();

        /**
         * Select ZX Pentagon 128 timings and settings.
         */
        void setPentagon();

        /**
         * Update pagination registers, and update the memory map.
         *
         * @param reg 0x00 for 128K ($7FFD) register, 0x01 for +3 ($1FFD) register.
         */
        void updatePage(uint_fast8_t reg);

        /**
         * Update page info in the memory map.
         *
         * @param page Memory map page (0: $0000-$3FFF, 1: $4000-7FFF, 2: $8000-$BFFF, 3: $C000-$FFFF).
         * @param bank Memory bank to use.
         * @param isRom Select a ROM page if true.
         * @param isContended Mark the page as contended.
         */
        void setPage(uint_fast8_t page, uint_fast8_t bank, bool isRom, bool isContended);

        /**
         * Select the video RAM page.
         *
         * @param page The page to be used by the ULA as video RAM.
         */
        void setScreenPage(uint_fast8_t page);

        /**
         * Select the snow data page.
         *
         * @param page This page will be read for snow effect data.
         */
        void setSnowPage(uint_fast8_t page);

        /**
         * Select current PSG from Z80 port written byte.
         */
        void psgSelect();

        /**
         * Read PSG selected register from current PSG.
         */
        void psgRead();

        /**
         * Write to PSG selected register from current PSG.
         */
        void psgWrite();

        /**
         * Select PSG register from current PSG.
         */
        void psgAddr();

        /**
         * Reset all PSGs.
         */
        void psgReset();

        /**
         * Clock all PSGs.
         */
        void psgClock();

        /**
         * Obtain sound samples from all PSGs.
         */
        void psgSample();

        /**
         * Select PSG type.
         *
         * @param aychip PSG type. (false = AY-8912-3, true = YM-2149)
         */
        void psgChip(bool aychip);

        /**
         * Mute or unmute all PSGs.
         */
        void psgPlaySound(bool play);

        /**
         * Check the Z80 status and paged ROM to check tape traps safely.
         */
        bool allowTapeTraps();

        /**
         * Mix and sample sound from all sources (Beeper, tape, PSG, Covox).
         */
        void sample();

        /**
         * Sample the Covox.
         */
        int dac();
};

// vim: et:sw=4:ts=4
