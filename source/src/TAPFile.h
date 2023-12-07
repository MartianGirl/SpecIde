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
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

/** TAPFile.h
 *
 * TAP file format container.
 *
 * This class generates data suitable for the Tape class.
 */

class TAPFile
{
    public:
        vector<uint8_t> fileData;

        TAPFile() {}

        void load(string const& fileName);
        void parse(
                vector<uint32_t> &pulseData,
                set<size_t> &indexData,
                set<size_t> &stopData);

        size_t pointer = 0;
        std::string name;
};

// vim: et:sw=4:ts=4:
