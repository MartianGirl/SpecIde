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

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>

#include "DSKFile.h"

using namespace std;

DSKFile::Track::Track(uint_fast16_t size) :
    magic {
        'T', 'r', 'a', 'c', 'k', '-', 'I', 'n', 'f', 'o', '\r', '\n', '\0'
    },
    trackSize(size),
    magicOk(false) {
}

bool DSKFile::Track::load(vector<uint8_t> const& data, size_t offset) {

    // Validate magic
    magicOk = equal(&magic[0x00], &magic[0x0C], &data[offset]);
    if (magicOk) {
        trackNumber = data[offset + 0x10];
        sideNumber = data[offset + 0x11];
        sectorSize = data[offset + 0x14];
        numSectors = data[offset + 0x15];
        gapLength = data[offset + 0x16];
        fillerByte = data[offset + 0x17];

        if ((0x18 + 8 * numSectors) > 0x100) {
            return false;
        }

        sectors.clear();

        size_t dataOffset = offset + 0x100;
        for (size_t ss = 0; ss < numSectors; ++ss) {
            Sector s;

            size_t secEntry = offset + 0x18 + 8 * ss;
            s.track = data[secEntry];
            s.side = data[secEntry + 1];
            s.sectorId = data[secEntry + 2];
            s.sectorSize = data[secEntry + 3];
            s.fdcStatusReg1 = data[secEntry + 4];
            s.fdcStatusReg2 = data[secEntry + 5];
            s.sectorLength = data[secEntry + 7] * 0x100 + data[secEntry + 6];

            size_t size =
                (s.sectorLength) ? s.sectorLength : (0x80 << s.sectorSize);
            if ((dataOffset + size) <= data.size()) {
                s.data.assign(&data[dataOffset], &data[dataOffset + size]);
                dataOffset += size;
            }

            // Opera 32K protection hack.
            if (s.sectorLength == 0 && s.sectorSize == 0x08
                    && s.track == 0x28 && s.sectorId == 0x08) {
                s.data.assign(0x2000, 0x00);
                for (size_t ii = 0; ii < 0x2000; ++ii) {
                    s.data[ii] = rand() & 0xFF;
                }
                for (size_t ii = 0; ii < sectors.back().sectorLength; ++ii) {
                    s.data[ii + 0x512] = sectors.back().data[ii];
                }
            }

            sectors.push_back(s);
        }
    }

    return magicOk;
}

void DSKFile::Track::dump(vector<uint8_t>& buffer) {

    size_t offset = buffer.size();
    buffer.insert(buffer.end(), 0x100, 0x00);

    copy(&magic[0x00], &magic[0x0D], &buffer[offset]);
    buffer[offset + 0x10] = trackNumber;
    buffer[offset + 0x11] = sideNumber;
    buffer[offset + 0x14] = sectorSize;
    buffer[offset + 0x15] = numSectors;
    buffer[offset + 0x16] = gapLength;
    buffer[offset + 0x17] = fillerByte;

    for (size_t ii = 0; ii < numSectors; ++ii) {
        size_t base = offset + 0x18 + 8 * ii;
        size_t seclen = sectors[ii].data.size();
        buffer[base] = sectors[ii].track;
        buffer[base + 0x01] = sectors[ii].side;
        buffer[base + 0x02] = sectors[ii].sectorId;
        buffer[base + 0x03] = sectors[ii].sectorSize;
        buffer[base + 0x04] = sectors[ii].fdcStatusReg1;
        buffer[base + 0x05] = sectors[ii].fdcStatusReg2;
        buffer[base + 0x06] = (seclen & 0x00FF);
        buffer[base + 0x07] = (seclen & 0xFF00) >> 8;
        buffer.insert(buffer.end(), sectors[ii].data.begin(), sectors[ii].data.end());
    }
}

uint8_t const DSKFile::specide[16] = "DSK by SpecIDE";

DSKFile::DSKFile() :
    stdMagic {
        'M', 'V', ' ', '-', ' ', 'C', 'P', 'C',
        'E', 'M', 'U', ' ', 'D', 'i', 's', 'k',
        '-', 'F', 'i', 'l', 'e', '\r', '\n', 'D',
        'i', 's', 'k', '-', 'I', 'n', 'f', 'o',
        '\r', '\n'
    },
    extMagic {
        'E', 'X', 'T', 'E', 'N', 'D', 'E', 'D',
        ' ', 'C', 'P', 'C', ' ', 'D', 'S', 'K',
        ' ', 'F', 'i', 'l', 'e', '\r', '\n', 'D',
        'i', 's', 'k', '-', 'I', 'n', 'f', 'o',
        '\r', '\n'
    }, 
    stdMagicOk(false), extMagicOk(false),
    validFile(false) {
}

void DSKFile::load(string const& fileName) {

    ifstream ifs(fileName.c_str(), ios::binary);
    if (ifs.good()) {
        fileData.clear();

        char c;
        while (ifs.get(c)) {
            fileData.push_back(c);
        }

        // Disk Info Block must be at least 0x100 bytes long.
        // If the file is shorter, then it is not valid.
        if (fileData.size() < 0x100) {
            return;
        }

        // Validate magic
        stdMagicOk = equal(&stdMagic[0x00], &stdMagic[0x08], fileData.begin());
        extMagicOk = equal(&extMagic[0x00], &extMagic[0x08], fileData.begin());
        validFile = stdMagicOk || extMagicOk;
    }

    if (validFile) {
        readNameOfCreator();
        readNumberOfTracks();
        readNumberOfSides();
        buildTrackSizeTable();
        if (validFile) {
            loadTracks();
        }
    } else {
        cout << fileName << ": Not a DSK image file." << endl;
    }
}

void DSKFile::readNameOfCreator() {

    copy(&fileData[0x22], &fileData[0x30], creator);
    creator[14] = creator[15] = '\0';
}

void DSKFile::readNumberOfTracks() {

    numTracks = fileData[0x30];
}

void DSKFile::readNumberOfSides() {

    numSides = fileData[0x31];
}

void DSKFile::buildTrackSizeTable() {

    size_t totalTracks = numTracks * numSides;
    trackSizeTable.clear();

    // We can't have so many tracks that we exceed the
    // Disk Information Block size.
    if (extMagicOk && (totalTracks + 0x34) > 0x100) {
        validFile = false;
        return;
    }

    if (stdMagicOk) {
        uint_fast16_t trackSize = fileData[0x33] * 0x100 + fileData[0x32];
        trackSizeTable.assign(totalTracks, trackSize);
    } else if (extMagicOk) {
        for (uint_fast16_t tt = 0; tt < totalTracks; ++tt) {
            uint_fast16_t trackSize = fileData[0x34 + tt] * 0x100;
            trackSizeTable.push_back(trackSize);
        }
    }
}

void DSKFile::loadTracks() {

    size_t totalTracks = numTracks * numSides;
    tracks.assign(totalTracks, Track());

    size_t offset = 0x100;
    for (size_t tt = 0; tt < totalTracks; ++tt) {
        tracks[tt].trackSize = trackSizeTable[tt];

        if (trackSizeTable[tt]) {
            tracks[tt].load(fileData, offset);
        }

        offset += trackSizeTable[tt];
    }
}

void DSKFile::save(string const& fileName) {

    size_t totalTracks = numTracks * numSides;
    if (totalTracks > 208) {
        cout << "Error: Too many tracks." << endl;
    }

    vector<uint8_t> buffer(&extMagic[0], &extMagic[34]);
    buffer.insert(buffer.end(), &specide[0], &specide[14]);
    buffer.push_back(numTracks);
    buffer.push_back(numSides);

    // Limit header to 0x100 bytes
    buffer.resize(0x100, 0);

    // Dump tracks
    for (size_t ii = 0; ii < totalTracks; ++ii) {
        buffer[0x34 + ii] = (trackSizeTable[ii] & 0xFF00) >> 8;
        if (trackSizeTable[ii]) {
            tracks[ii].dump(buffer);
        }
    }

    ofstream ofs(fileName.c_str(), ios::binary);
    if (ofs.good()) {
        for (size_t ii = 0; ii < buffer.size(); ++ii) {
            ofs.put(buffer[ii]);
        }
    }
}
// vim: et:sw=4:ts=4:
