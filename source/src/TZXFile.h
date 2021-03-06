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

#include <cstdint>
#include <set>
#include <sstream>
#include <string>
#include <vector>

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
                  majorVersion(0), minorVersion(0),
                  pointer(0), loopStart(0), loopCounter(0) {}

        uint8_t magic[8];
        bool magicIsOk;
        uint8_t majorVersion, minorVersion;

        std::vector<uint8_t> fileData;
        std::vector<uint8_t> romData;

        size_t pointer;
        size_t loopStart;
        size_t loopCounter;

        std::stringstream ss;   // For reporting.

        void load(std::string const& fileName);
        void parse(
                std::vector<size_t> &pulseData,
                std::set<size_t> &indexData,
                std::set<size_t> &stopData,
                std::set<size_t> &stopIf48K);

        size_t dumpArchiveInfo();
        size_t dumpComment();
        size_t dumpMessage();

        void loadSymbolData(size_t base,
                size_t& numSym, size_t& maxLen, size_t& alphaSize);
        size_t loadSymbolAlphabet(size_t base, size_t numSym, size_t maxLen,
                std::vector<size_t>& alphabet);
        size_t dumpPilotStream(size_t base, size_t numSym,
                std::vector<size_t> const& alphabet, std::vector<size_t>& data);
        size_t dumpDataStream(size_t base, size_t numSym, size_t bps,
                std::vector<size_t> const& alphabet, std::vector<size_t>& data);
        void pushSymbol(size_t rep, size_t sym,
                std::vector<size_t> const& alphabet, std::vector<size_t>& data);
        void addPause(size_t pause, std::vector<size_t>& data);

};

// vim: et:sw=4:ts=4:
