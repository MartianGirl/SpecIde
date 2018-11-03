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

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

/** DSKFile.h
 *
 * DSK file format implementation.
 *
 * This class loads a DSK file.
 */

class DSKFile
{
    public:

        class Track
        {
            public:
                class Sector
                {
                    public:
                        uint_fast8_t track;
                        uint_fast8_t side;
                        uint_fast8_t sectorId;
                        uint_fast8_t sectorSize;
                        uint_fast8_t fdcStatusReg1;
                        uint_fast8_t fdcStatusReg2;
                        uint_fast16_t sectorLength;

                        vector<uint8_t> data;
                };

                Track(uint_fast16_t size = 0) :
                    magic {
                        'T', 'r', 'a', 'c', 'k', '-', 'I', 'n',
                            'f', 'o', '\r', '\n'},
                    trackSize(size),
                    magicOk(false) {}

                uint_fast8_t magic[12];
                uint_fast8_t trackNumber;
                uint_fast8_t sideNumber;
                uint_fast8_t sectorSize;
                uint_fast8_t numSectors;
                uint_fast8_t gapLength;
                uint_fast8_t fillerByte;
                uint_fast16_t trackSize;

                vector<Sector> sectors;

                bool magicOk;

                bool load(vector<uint8_t> const& data, uint_fast32_t offset)
                {
                    magicOk = false;

                    // Validate magic
                    if (equal(&magic[0x00], &magic[0x0C], &data[offset]))
                        magicOk = true;

                    if (magicOk)
                    {
                        trackNumber = data[offset + 0x10];
                        sideNumber = data[offset + 0x11];
                        sectorSize = data[offset + 0x14];
                        numSectors = data[offset + 0x15];
                        gapLength = data[offset + 0x16];
                        fillerByte = data[offset + 0x17];

                        if ((0x18 + 8 * numSectors) > 0x100)
                            return false;

                        sectors.clear();
                        uint_fast32_t dataOffset = offset + 0x100;
                        for (uint_fast8_t sector = 0; sector < numSectors; ++sector)
                        {
                            Sector s;
                            uint_fast32_t secEntry = offset + 0x18 + 8 * sector;

                            s.track = data[secEntry];
                            s.side = data[secEntry + 1];
                            s.sectorId = data[secEntry + 2];
                            s.sectorSize = data[secEntry + 3];
                            s.fdcStatusReg1 = data[secEntry + 4];
                            s.fdcStatusReg2 = data[secEntry + 5];
                            s.sectorLength =
                                data[secEntry + 7] * 0x100
                                + data[secEntry + 6];

                            uint_fast16_t size =
                                (s.sectorLength) ? s.sectorLength : (0x80 << s.sectorSize);

                            s.data.assign(
                                        &data[dataOffset],
                                        &data[dataOffset + size]);
                            dataOffset += size;

                            sectors.push_back(s);
                        }
                    }

                    return true;
                }

                void readSector(vector<uint8_t>& data, uint_fast8_t id)
                {
                    for (vector<Sector>::iterator it = sectors.begin();
                            it != sectors.end();
                            ++it)
                    {
                        if (it->sectorId == id)
                            data.insert(data.end(), it->data.begin(), it->data.end());
                    }
                }

        };

        DSKFile() :
            stdMagic {
                'M', 'V', ' ', '-', ' ', 'C', 'P', 'C',
                    'E', 'M', 'U', ' ', 'D', 'i', 's', 'k',
                    '-', 'F', 'i', 'l', 'e', '\r', '\n', 'D',
                    'i', 's', 'k', '-', 'I', 'n', 'f', 'o',
                    '\r', '\n' },
            extMagic {
                'E', 'X', 'T', 'E', 'N', 'D', 'E', 'D',
                ' ', 'C', 'P', 'C', ' ', 'D', 'S', 'K',
                ' ', 'F', 'i', 'l', 'e', '\r', '\n', 'D',
                'i', 's', 'k', '-', 'I', 'n', 'f', 'o',
                '\r', '\n' }, 
            stdMagicOk(false), extMagicOk(false),
            validFile(false)
            {}

        uint_fast8_t stdMagic[34];
        uint_fast8_t extMagic[34];
        uint_fast8_t creator[16];
        uint_fast8_t numTracks;
        uint_fast8_t numSides;
        vector<uint_fast16_t> trackSizeTable;
        vector<Track> tracks;

        bool stdMagicOk;
        bool extMagicOk;
        bool validFile;

        vector<uint8_t> fileData;

        void load(string const& fileName)
        {
            char c;

            stdMagicOk = extMagicOk = false;
            validFile = false;

            ifstream ifs(fileName.c_str(), ifstream::binary);
            if (ifs.good())
            {
                fileData.clear();
                while (ifs.get(c))
                    fileData.push_back(c);

                // Disk Info Block must be at least 0x100 bytes long.
                // If the file is shorter, then it is not valid.
                if (fileData.size() < 0x100)
                    return;

                // Validate magic
                if (equal(&stdMagic[0x00], &stdMagic[0x08], fileData.begin()))
                {
                    stdMagicOk = true;
                    printf("%s: Standard DSK image detected.\n", fileName.c_str());
                }
                else if (equal(&extMagic[0x00], &extMagic[0x08], fileData.begin()))
                {
                    extMagicOk = true;
                    printf("%s: Extended DSK image detected.\n", fileName.c_str());
                }
                else
                {
                    printf("%s: Not a DSK image file.\n", fileName.c_str());
                }
            }

            if (stdMagicOk || extMagicOk)
            {
                validFile = true;
                readNameOfCreator();
                readNumberOfTracks();
                readNumberOfSides();
                buildTrackSizeTable();
                loadTracks();
            }
        }

        void readNameOfCreator()
        {
            copy(&fileData[0x22], &fileData[0x30], creator);
            creator[14] = creator[15] = '\0';
            printf("Creator: %s\n", creator);
        }

        void readNumberOfTracks()
        {
            numTracks = fileData[0x30];
            printf("Number of tracks: %u\n", numTracks);
        }

        void readNumberOfSides()
        {
            numSides = fileData[0x31];
            printf("Number of sides: %u\n", numSides);
        }

        void buildTrackSizeTable()
        {
            uint_fast16_t totalTracks = numTracks * numSides;
            trackSizeTable.clear();

            // We can't have so many tracks that we exceed the
            // Disk Information Block size.
            if (extMagicOk && (totalTracks * 2 + 0x34) > 0x100)
            {
                printf("Invalid DSK file: Too many tracks.\n");
                validFile = false;
                return;
            }

            if (stdMagicOk)
            {
                uint_fast16_t trackSize =
                    fileData[0x33] * 0x100 + fileData[0x32];
                trackSizeTable.assign(totalTracks, trackSize);
            }
            else if (extMagicOk)
            {
                for (uint_fast16_t track = 0; track < totalTracks; ++track)
                {
                    uint_fast16_t trackSize =
                        fileData[0x34 + track] * 0x100;
                    trackSizeTable.push_back(trackSize);
                }
            }

            for (unsigned int track = 0; track < numTracks; ++track)
            {
                for (unsigned int side = 0; side < numSides; ++side)
                {
                    unsigned int size = trackSizeTable[track * numSides + side];
                    printf("Track %u, Side %u size: %u    ", track, side, size);
                }
                printf("\n");
            }
        }

        void loadTracks()
        {
            uint_fast16_t totalTracks = numTracks * numSides;
            tracks.assign(totalTracks, Track());

            uint_fast32_t offset = 0x100;
            for (uint_fast16_t track = 0; track < totalTracks; ++track)
            {
                tracks[track].load(fileData, offset);
                offset += trackSizeTable[track];
            }
        }
};


// vim: et:sw=4:ts=4:
