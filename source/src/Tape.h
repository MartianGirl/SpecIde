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

class Tape
{
    public:
        vector<size_t> pulseData;   // Pulse data, in samples per pulse.
        set<size_t> indexData;      // Indexes relative to pulse data.
        set<size_t> stopData;       // Stop points, relative to pulse data.
        set<size_t> stopIf48K;      // Stop points only for 48K mode.

        vector<uint8_t> tapData;    // Raw TAP data, just for tape load trap.
        size_t tapPointer;          // Raw TAP pointer.

        vector<uint8_t> saveData;
        vector<uint8_t> loadData;
        bool useSaveData;
        unsigned int tapes;

        size_t pointer;         // Index to pulse data.
        size_t index;           // Last reached index in the tape.
        size_t sample;          // Sample counter.
        bool playing;
        uint8_t level;          // Tape output level.
        size_t counter;         // Cassette counter :-)

        bool is48K;             // For deciding if we stop or not :)

        Tape() :
            tapPointer(0),
            useSaveData(false), tapes(0),
            pointer(0), index(0), sample(0),
            playing(false),
            level(0x7F), is48K(true) {}

        void loadTzx(string const& fileName);
        void loadTap(string const& fileName);
        void loadCsw(string const& fileName);
        void updateFlashTap();

        void play()
        {
            playing = !playing;
            cout << (playing ? "Playing." : "Stopped.") << endl;
        }

        void rewind(size_t position = 0)
        {
            playing = false; sample = 0; pointer = position; level = 0x7F;
            if (position == 0) tapPointer = 0;
            cout << "Rewind to " << position << "..." << endl;
        }

        void resetCounter()
        {
            counter = pointer + 1;
            cout << "Set counter at " << pointer << "..." << endl;
        }

        uint_fast8_t advance();
        void next() { pointer = *(indexData.upper_bound(pointer + 1)); }
        void prev() { pointer = *(indexData.lower_bound(pointer - 1)); }

        // Functions for tap blocks
        uint_fast8_t getBlockByte(size_t offset)
        {
            return tapData[tapPointer + offset];
        }

        bool foundTapBlock(uint_fast8_t flag)
        {
            bool res;
            if (getBlockByte(2) == flag)
            {
                cout << "Loading - ";
                res = true;
            }
            else
            {
                cout << "Found   - ";
                res = false;
            }

            cout << "Flag: " << setw(3) << static_cast<size_t>(getBlockByte(2))
                << "  Length: " << setw(5) << getBlockLength() << endl;
            return res;
        }

        uint16_t getBlockLength()
        {
            return getBlockByte(1) * 0x100 + getBlockByte(0);
        }

        void nextTapBlock()
        {
            tapPointer += 2 + getBlockLength();

            if (tapPointer >= tapData.size())
                tapPointer = 0;

            next();
        }

        void selectTapData()
        {
            if (useSaveData)
            {
                cout << "FlashTAP is save tape." << endl;
                tapData.assign(saveData.begin(), saveData.end());
            }
            else
            {
                cout << "FlashTAP is load tape." << endl;
                tapData.assign(loadData.begin(), loadData.end());
            }

            tapPointer = 0;
        }

        void writeSaveData()
        {
            char name[256];
            snprintf(name, 256, "savetape%02u.tap", tapes);
            cout << "Saving to " << name << endl;

            char *data = reinterpret_cast<char*>(&saveData[0]);
            size_t size = saveData.size();
            ofstream ofs(name, std::ofstream::binary);
            ofs.write(data, size);
            ofs.close();

            tapes = (tapes + 1) % 100;
        }

        void clearSaveData()
        {
            cout << "Clearing save buffer." << endl;
            saveData.clear();
        }

        void appendLoadData()
        {
            cout << "Dumping FlashTAP to save buffer." << endl;
            saveData.insert(saveData.end(), tapData.begin(), tapData.end());
        }
};

// vim: et:sw=4:ts=4:
