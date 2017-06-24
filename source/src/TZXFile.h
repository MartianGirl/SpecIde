#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/** TZXFile.h
 *
 * TZX file format implementation.
 *
 * This class loads a TZX file and generates pulses that will be fed to the
 * Spectrum EAR port.
 */

enum class Stages
{
    STOPPED,
    PILOT,
    SYNC_ONE,
    SYNC_TWO,
    DATA,
    PAUSE,
    DONE
};

class TZXBlock
{
    public:
        size_t id;
        size_t pilotPulseLength;
        size_t syncOnePulseLength;
        size_t syncTwoPulseLength;
        size_t zeroPulseLength;
        size_t onePulseLength;
        
        size_t pilotLength;

        size_t usedBitsInLastByte;
        size_t pause;

        vector<uint8_t> data;
};

class TZXFile
{
    public:
        uint8_t magic[8];

        vector<uint8_t> data;
        uint8_t major, minor;

        size_t pointer;
        size_t nextBlock;

        TZXBlock block;
        size_t pulseLength;
        size_t numPulses;
        size_t numSamples;
        size_t numBit;
        size_t numByte;

        uint8_t byte;
        uint8_t sample;
        Stages stage;
        bool playing;

        TZXFile();
        bool load(string const& fileName);
        void getNextSample();
        void getNextPulse();

        void start();
        void stop();
        uint8_t play();
        bool rewind();
        bool advance();
        bool isLastBlock();
        bool getBlock();
        void dumpBlockInfo();
};

// vim: et:sw=4:ts=4:
