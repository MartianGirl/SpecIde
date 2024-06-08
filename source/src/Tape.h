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

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "TAPFile.h"
#include "TZXFile.h"
#include "CSWFile.h"

using namespace std;

/** Tape.h
 *
 * Tape streaming from different sources.
 *
 * This class generates a square wave from different data sources.
 */

class Tape {

    public:
        vector<uint32_t> pulseData; // Pulse data, in samples per pulse.
        set<size_t> indexData;      // Indexes relative to pulse data.
        set<size_t> stopData;       // Stop points, relative to pulse data.
        set<size_t> stopIf48K;      // Stop points only for 48K mode.

        vector<uint8_t> tapData;    // Raw TAP data, just for tape load trap.
        size_t tapPointer = 0;      // Raw TAP pointer.

        vector<uint8_t> saveData;
        vector<uint8_t> loadData;
        bool useSaveData = false;
        uint32_t tapes = 0;

        size_t pointer = 0;         // Index to pulse data.
        size_t index = 0;           // Last reached index in the tape.
        size_t counter = 0;         // Cassette counter :-)

        uint32_t sample = 0;        // Sample counter.
        uint8_t level = 0x00;       // Tape output level.

        /**
         * TZX tapes assume a 3.5MHz clock. This value adjusts the
         * tape speed for different CPU clocks.
         * ZX Spectrum (@ ~3.5MHz): 1.0.
         * Amstrad CPC: (@ 4MHz) 1.16.
         */
        double speed = 1.0;         // Tape speed factor (1.00 for ZX, 1.16 for CPC)

        bool playing = false;       // Is tape playing?
        bool is48K = true;          // For deciding if we stop or not :)

        Tape() {}

        void loadTzx(string const& fileName);
        void loadCdt(string const& fileName);
        void loadTap(string const& fileName);
        void loadCsw(string const& fileName);
        void updateFlashTap();

        void play();
        void rewind(size_t position = 0);
        void resetCounter();

        void advance();
        void next();
        void prev();

        // Functions for tap blocks
        uint_fast8_t getBlockByte(size_t offset);
        bool foundTapBlock(uint_fast8_t flag);
        uint16_t getBlockLength();
        void nextTapBlock();
        void selectTapData();
        void writeSaveData();
        void clearSaveData();
        void appendLoadData();
};

// vim: et:sw=4:ts=4:
