/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2019.
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

/** DSKFile.h
 *
 * DSK file format implementation.
 *
 * This class loads a DSK file.
 */

class DSKFile {

    public:
        class Track {

            public:
                class Sector {

                    public:
                        uint_fast8_t track;
                        uint_fast8_t side;
                        uint_fast8_t sectorId;
                        uint_fast8_t sectorSize;
                        uint_fast8_t fdcStatusReg1;
                        uint_fast8_t fdcStatusReg2;
                        uint_fast16_t sectorLength;

                        std::vector<uint8_t> data;
                };

                Track(uint_fast16_t size = 0);

                uint_fast8_t magic[13];
                uint_fast8_t trackNumber;
                uint_fast8_t sideNumber;
                uint_fast8_t sectorSize;
                uint_fast8_t numSectors;
                uint_fast8_t gapLength;
                uint_fast8_t fillerByte;
                uint_fast16_t trackSize;

                std::vector<Sector> sectors;

                bool magicOk;

                bool load(std::vector<uint8_t> const& data, uint_fast32_t offset);
                void dump(std::vector<uint8_t>& data);
        };

        DSKFile();

        static uint8_t const specide[16];

        uint8_t stdMagic[34];
        uint8_t extMagic[34];
        uint8_t creator[16];
        uint_fast8_t numTracks;
        uint_fast8_t numSides;
        std::vector<uint_fast16_t> trackSizeTable;
        std::vector<Track> tracks;

        bool stdMagicOk;
        bool extMagicOk;
        bool validFile;

        std::vector<uint8_t> fileData;

        void load(std::string const& fileName);
        void save(std::string const& fileName);
        void readNameOfCreator();
        void readNumberOfTracks();
        void readNumberOfSides();
        void buildTrackSizeTable();
        void loadTracks();
};

// vim: et:sw=4:ts=4:
