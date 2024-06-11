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
#include <iostream>
#include <set>
#include <vector>

class CSWFile {
    public:
        CSWFile() :
            magic {
                'C', 'o', 'm', 'p', 'r', 'e', 's', 's',
                    'e', 'd', ' ', 'S', 'q', 'u', 'a', 'r',
                    'e', ' ', 'W', 'a', 'v', 'e', 0x1A
            } {}

        uint8_t magic[23];
        bool magicIsOk = false;
        uint8_t majorVersion = 0;
        uint8_t minorVersion = 0;

        double rate;
        uint8_t compression;
        uint8_t flags;

        std::vector<uint8_t> fileData;
        std::vector<uint8_t> romData;

        void load(std::string const& fileName);
        void parse(std::vector<uint32_t> &pulseData,
                std::set<size_t> &indexData,
                std::set<size_t> &stopData);
};

// vim: et:sw=4:ts=4
