#pragma once

#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "TAPFile.h"
#include "TZXFile.h"

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
        size_t tapes;

        size_t pointer;         // Index to pulse data.
        size_t index;           // Last reached index in the tape.
        size_t sample;          // Sample counter.
        bool playing;
        bool clock;
        uint8_t level;          // Tape output level.
        size_t counter;         // Cassette counter :-)

        bool is48K;             // For deciding if we stop or not :)

        Tape() :
            tapPointer(0),
            useSaveData(false), tapes(0),
            pointer(0), index(0), sample(0),
            playing(false), clock(false), level(0x00), is48K(true) {}

        void load(string const& fileName);

        void play()
        {
            playing = !playing;
            level ^= 0x80;
            if (playing)
                printf("Playing.\n");
            else
                printf("Stopped.\n");
        }

        void rewind(size_t position = 0)
        {
            playing = false; sample = 0; pointer = position; level = 0x00;
            if (position == 0) tapPointer = 0;
            printf("Rewind to %ld...\n", position);
        }

        void resetCounter()
        {
            counter = pointer + 1;
            printf("Set counter at %ld...\n", pointer);
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
                printf("Loading - ");
                res = true;
            }
            else
            {
                printf("Found   - ");
                res = false;
            }

            printf("Flag: %3d  Length: %5d\n",
                    getBlockByte(2), getBlockLength());
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
                printf("FlashTAP is save tape.\n");
                tapData.assign(saveData.begin(), saveData.end());
            }
            else
            {
                printf("FlashTAP is load tape.\n");
                tapData.assign(loadData.begin(), loadData.end());
            }

            tapPointer = 0;
        }

        void writeSaveData()
        {
            ++tapes;

            char name[256];
            snprintf(name, 256, "savetape%02ld.tap", tapes);
            printf("Saving to %s\n", name);

            char *data = reinterpret_cast<char*>(&saveData[0]);
            size_t size = saveData.size();
            ofstream ofs(name, std::ofstream::binary);
            ofs.write(data, size);
            ofs.close();
        }

        void clearSaveData()
        {
            printf("Clearing save buffer.\n");
            saveData.clear();
        }

        void appendLoadData()
        {
            printf("Dumping FlashTAP to save buffer.\n");
            saveData.insert(saveData.end(), tapData.begin(), tapData.end());
        }
};

// vim: et:sw=4:ts=4:
