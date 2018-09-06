#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
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

class TZXFile
{
    public:
        TZXFile() :
            magic { 'Z', 'X', 'T', 'a', 'p', 'e', '!', 0x1A },
                  magicIsOk(false),
                  firstLeadTone(false),
                  majorVersion(0), minorVersion(0),
                  pointer(0), loopStart(0), loopCounter(0) {}

        uint8_t magic[8];
        bool magicIsOk;
        bool firstLeadTone;
        uint8_t majorVersion, minorVersion;

        vector<uint8_t> fileData;
        vector<uint8_t> romData;

        size_t pointer;
        size_t loopStart;
        size_t loopCounter;

        std::stringstream ss;   // For reporting.

        void load(string const& fileName);
        void parse(
                vector<size_t> &pulseData,
                set<size_t> &indexData,
                set<size_t> &stopData,
                set<size_t> &stopIf48K);

        size_t dumpArchiveInfo();
        size_t dumpComment();
        size_t dumpMessage();

        void loadSymbolData(size_t base,
                size_t& numSym, size_t& maxLen, size_t& alphaSize);
        size_t loadSymbolAlphabet(size_t base, size_t numSym, size_t maxLen,
                vector<size_t>& alphabet);
        size_t dumpPilotStream(size_t base, size_t numSym,
                vector<size_t> const& alphabet, vector<size_t>& data);
        size_t dumpDataStream(size_t base, size_t numSym, size_t bps,
                vector<size_t> const& alphabet, vector<size_t>& data);
        void pushSymbol(size_t rep, size_t sym,
                vector<size_t> const& alphabet, vector<size_t>& data);
        void addPause(size_t pause, vector<size_t>& data);

};

// vim: et:sw=4:ts=4:
