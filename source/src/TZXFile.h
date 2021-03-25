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

class TZXFile {

    public:
        TZXFile() :
            magic { 'Z', 'X', 'T', 'a', 'p', 'e', '!', 0x1A } {}

        uint8_t magic[8];
        bool magicIsOk = false;
        uint8_t majorVersion = 0;
        uint8_t minorVersion = 0;

        std::vector<uint8_t> fileData;
        std::vector<uint8_t> romData;

        size_t pointer = 0;
        size_t loopStart = 0;
        size_t loopCounter = 0;

        std::stringstream ss;   // For reporting.

        void load(std::string const& fileName);
        void parse(
                std::vector<uint32_t> &pulseData,
                std::set<size_t> &indexData,
                std::set<size_t> &stopData,
                std::set<size_t> &stopIf48K);

        size_t dumpArchiveInfo();
        size_t dumpComment();
        size_t dumpMessage();

        void loadSymbolData(size_t base,
                uint32_t& numSym, uint32_t& maxLen, uint32_t& alphaSize);
        size_t loadSymbolAlphabet(size_t base, uint32_t numSym, uint32_t maxLen,
                std::vector<uint32_t>& alphabet);
        size_t dumpPilotStream(size_t base, uint32_t numSym,
                std::vector<uint32_t> const& alphabet, std::vector<uint32_t>& data);
        size_t dumpDataStream(size_t base, uint32_t numSym, uint32_t bps,
                std::vector<uint32_t> const& alphabet, std::vector<uint32_t>& data);
        void pushSymbol(uint32_t rep, uint32_t sym,
                std::vector<uint32_t> const& alphabet, std::vector<uint32_t>& data);
        void addPause(uint32_t pause, std::vector<uint32_t>& data);
};

// vim: et:sw=4:ts=4:
