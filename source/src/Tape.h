#pragma once

#include <iostream>
#include <set>
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

        vector<uint8_t> tapData;     // Raw TAP data, just for tape load trap.
        size_t tapPointer;          // Raw TAP pointer.

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
            pointer(0), index(0), sample(0),
            playing(false), clock(false), level(0x00), is48K(true) {}

        void load(string const& fileName);

        void play()
        {
            playing = !playing;
            level ^= 0x80;
            cout << ((playing) ? "Playing" : "Stopped") << endl;
        }

        void rewind(size_t position = 0)
        {
            playing = false; sample = 0; pointer = position; level = 0x00;
            cout << "Rewind to " << position << "..." << endl;
        }

        void resetCounter()
        {
            counter = pointer + 1;
            cout << "Set counter at " << pointer << endl;
        }

        uint_fast8_t advance();
        void next() {}
        void prev() {}

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
                cout << "Loading block.";
                res = true;
            }
            else
            {
                cout << "Found block.";
                res = false;
            }

            cout << " Flag: " << static_cast<size_t>(getBlockByte(2));
            cout << " Length: " << getBlockLength() << endl;
            return res;
        }

        size_t getBlockLength()
        {
            return getBlockByte(1) * 0x100 + getBlockByte(0);
        }

        void nextTapBlock()
        {
            tapPointer += 2 + getBlockLength();

            if (tapPointer >= tapData.size())
                tapPointer = 0;
        }
};

// vim: et:sw=4:ts=4:
