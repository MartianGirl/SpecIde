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
                        size_t dataOffset = offset + 0x100;

                        for (size_t ss = 0; ss < numSectors; ++ss)
                        {
                            Sector s;
                            size_t secEntry = offset + 0x18 + 8 * ss;

                            s.track = data[secEntry];
                            s.side = data[secEntry + 1];
                            s.sectorId = data[secEntry + 2];
                            s.sectorSize = data[secEntry + 3];
                            s.fdcStatusReg1 = data[secEntry + 4];
                            s.fdcStatusReg2 = data[secEntry + 5];
                            s.sectorLength =
                                data[secEntry + 7] * 0x100
                                + data[secEntry + 6];

                            // cout << hex << setw(2) << setfill('0');
                            // cout << "Sector: " << ss << " ";
                            // cout << "Track: " << static_cast<size_t>(s.track) << " ";
                            // cout << "Side: " << static_cast<size_t>(s.side) << " ";
                            // cout << "Id: " << static_cast<size_t>(s.sectorId) << " ";
                            // cout << "Size: " << static_cast<size_t>(0x80 << s.sectorSize) << " ";
                            // cout << "ST1: " << static_cast<size_t>(s.fdcStatusReg1) << " ";
                            // cout << "ST2: " << static_cast<size_t>(s.fdcStatusReg2) << " ";
                            // cout << "Actual len: " << static_cast<size_t>(s.sectorLength) << " ";

                            size_t size =
                                (s.sectorLength) ? s.sectorLength : (0x80 << s.sectorSize);

                            if ((dataOffset + size) <= data.size())
                            {
                                s.data.assign(
                                        &data[dataOffset],
                                        &data[dataOffset + size]);
                                dataOffset += size;
                            }
                            // else
                            // {
                                // cout << "No data. ";
                            // }

                            // Opera 32K protection hack.
                            if (s.sectorLength == 0 && s.sectorSize == 0x08
                                    && s.track == 0x28 && s.sectorId == 0x08)
                            {
                                // cout << "Opera 32K sector detected. ";
                                s.data.assign(0x2000, 0x00);
                                for (size_t ii = 0; ii < 0x2000; ++ii)
                                    s.data[ii] = rand() & 0xFF;
                                for (size_t ii = 0; ii < sectors.back().sectorLength; ++ii)
                                    s.data[ii + 0x512] = sectors.back().data[ii];
                            }
                            // cout << endl;

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
                    cout << fileName << ": Standard DSK image detected." << endl;
                }
                else if (equal(&extMagic[0x00], &extMagic[0x08], fileData.begin()))
                {
                    extMagicOk = true;
                    cout << fileName << ": Extended DSK image detected." << endl;
                }
                else
                {
                    cout << fileName << ": Not a DSK image file." << endl;
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
            cout << "Creator: " << creator << endl;
        }

        void readNumberOfTracks()
        {
            numTracks = fileData[0x30];
            cout << "Number of tracks: " << static_cast<size_t>(numTracks) << endl;
        }

        void readNumberOfSides()
        {
            numSides = fileData[0x31];
            cout << "Number of sides " << static_cast<size_t>(numSides) << endl;
        }

        void buildTrackSizeTable()
        {
            size_t totalTracks = numTracks * numSides;
            trackSizeTable.clear();

            // We can't have so many tracks that we exceed the
            // Disk Information Block size.
            if (extMagicOk && (totalTracks * 2 + 0x34) > 0x100)
            {
                cout << "Invalid DSK file: Too many tracks." << endl;
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

            // for (size_t track = 0; track < numTracks; ++track)
            // {
                // for (size_t side = 0; side < numSides; ++side)
                // {
                    // size_t size = trackSizeTable[track * numSides + side];
                    // cout << "Track " << track << ", Side " << side;
                    // cout << " size: " << size << "    ";
                // }
                // cout << endl;
            // }
        }

        void loadTracks()
        {
            size_t totalTracks = numTracks * numSides;
            tracks.assign(totalTracks, Track());

            size_t offset = 0x100;
            for (size_t tt = 0; tt < totalTracks; ++tt)
            {
                tracks[tt].trackSize = trackSizeTable[tt];

                if (trackSizeTable[tt])
                    tracks[tt].load(fileData, offset);

                offset += trackSizeTable[tt];

                // cout << hex << setw(2) << setfill('0');
                // cout << "Track: " << static_cast<size_t>(tracks[tt].trackNumber) << " ";
                // cout << "Side: " << static_cast<size_t>(tracks[tt].sideNumber) << " ";
                // cout << "Sector size: " << static_cast<size_t>(0x80 << tracks[tt].sectorSize) << " ";
                // cout << "Num sectors: " << static_cast<size_t>(tracks[tt].numSectors) << " ";
                // cout << "Gap length: " << static_cast<size_t>(tracks[tt].gapLength) << " ";
                // cout << "Track size: " << static_cast<size_t>(tracks[tt].trackSize) << endl;
            }
        }
};


// vim: et:sw=4:ts=4:
