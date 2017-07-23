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

        size_t pointer;         // Index to pulse data.
        size_t index;           // Last reached index in the tape.
        size_t sample;          // Sample counter.
        bool playing;
        uint8_t level;          // Tape output level.

        Tape() :
            pointer(0), index(0), sample(0), playing(false), level(0x00) {}

        void load(string const& fileName);

        void play()
        {
            playing = !playing;
            cout << ((playing) ? "Playing" : "Stopped") << endl;
        }
        void rewind()
        {
            playing = false; sample = 0; pointer = 0;
            cout << "Rewind..." << endl;
        }
        void advance();
        void next() {}
        void prev() {}
};

// vim: et:sw=4:ts=4:
