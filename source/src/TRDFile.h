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
#include <string>
#include <vector>

/** TRDFile.h
 *
 * TRD and SCL file format implementation.
 *
 * This class loads a TRD or a SCL image.
 */

class TRDFile {

    public:
        TRDFile();

        uint_fast8_t numTracks;
        uint_fast8_t numSides;

        uint_fast8_t freeSpaceAddress[2];
        uint_fast8_t diskType;
        uint_fast8_t numFiles;
        uint_fast8_t numDeleted;
        uint_fast16_t numFreeSectors;
        uint8_t diskLabel[8];

        std::vector<uint8_t> diskData;
        std::vector<uint8_t> fileData;
        bool diskOk = false;

        uint8_t sclMagic[8] = {'S', 'I', 'N', 'C', 'L', 'A', 'I', 'R'};
        bool sclMagicOk = false;

        void loadTRD(std::string const& fileName);
        void loadSCL(std::string const& fileName);
        void save(std::string const& fileName);
};

// vim: et:sw=4:ts=4:
