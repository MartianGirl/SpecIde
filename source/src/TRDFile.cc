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

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>

#include "TRDFile.h"

using namespace std;

void TRDFile::loadTRD(string const& fileName) {

    ifstream ifs(fileName.c_str(), ios::binary);
    if (ifs.good()) {
        diskData.clear();

        char c;
        while (ifs.get(c)) {
            diskData.push_back(c);
        }
    }

    // TR-DOS disks are 256 BPS, 16 SPT. Info of geometry is in track 0.
    // Disk Info Block must be at least 0x100 bytes long.
    // File descriptors are in h0t0s1 to h0t0s8.
    // Disk info is in h0t0s9. The rest of h0t0 is unused.
    // The file should have at least 9 sectors, that is, 256 * 9 bytes.
    if (diskData.size() < 0x900) {
        return;
    }

    // First byte of disk info is 0x00 (end of catalogue)
    // Byte at offset 0xE7 is 0x10 (TR-DOS ID)
    if (diskData[0x900] == 0 || diskData[0x9E7] == 0x10) {
        diskType = diskData[0x9E3];

        switch (diskType) {
            case 0x16:
                diskOk = true;
                numTracks = 80;
                numSides = 2;
                break;

            case 0x17:
                diskOk = true;
                numTracks = 40;
                numSides = 2;
                break;

            case 0x18:
                diskOk = true;
                numTracks = 80;
                numSides = 1;
                break;

            case 0x19:
                diskOk = true;
                numTracks = 40;
                numSides = 1;
                break;

            default:
                break;;
        }
    }

    if (diskOk) {
        diskData.resize(0x1000 * numTracks * numSides);
        freeSpaceAddress[0] = diskData[0x9E1];
        freeSpaceAddress[1] = diskData[0x9E2];
        numFiles = diskData[0x9E4];
        numFreeSectors = diskData[0x9E5] * 0x100 + diskData[0x9E6];
        numDeleted = diskData[0x9F4];
        copy(&diskData[0x9F5], &diskData[0x9FC], &diskLabel[0]);
    } else {
        cout << fileName << ": Not a valid TRD image file." << endl;
    }
}

void TRDFile::loadSCL(string const& fileName) {

    ifstream ifs(fileName.c_str(), ios::binary);
    if (ifs.good()) {
        fileData.clear();

        char c;
        while (ifs.get(c)) {
            fileData.push_back(c);
        }
    }

    sclMagicOk = equal(&sclMagic[0x00], &sclMagic[0x08], fileData.begin());

    if (sclMagicOk) {
        numFiles = fileData[9];
    }

}

// vim: et:sw=4:ts=4:
