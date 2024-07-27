/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2024.
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

/** PZXFile.h
 *
 * PZX file format implementation.
 *
 * This class loads a PZX file and generates pulses that will be fed to the
 * Spectrum EAR port.
 */

class PZXFile {

    public:
        PZXFile() {}

        std::string name;

        uint8_t majorVersion = 0;
        uint8_t minorVersion = 0;

        std::vector<uint8_t> fileData;
        std::vector<uint8_t> romData;

        std::stringstream ss;   // For reporting.

        void load(std::string const& fileName);
        void parse(
                std::vector<uint32_t> &pulseData,
                std::set<size_t> &indexData,
                std::set<size_t> &stopData,
                std::set<size_t> &stopIf48K);

};

// vim: et:sw=4:ts=4:
