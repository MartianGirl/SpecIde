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

#include "CSWFile.h"
#include "Utils.h"

#include <fstream>

using namespace std;

void CSWFile::load(std::string const& fileName) {

    char c;

    magicIsOk = false;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good()) {
        fileData.clear();
        while (ifs.get(c)) {
            fileData.push_back(c);
        }

        // CSW header is:
        // 0x00 - "Compressed Square Wave"
        // 0x16 - 0x1A
        // 0x17 - Major version number
        // 0x18 - Minor version number
        //
        // If the magic checks, store the version numbers.
        if (equal(&magic[0x00], &magic[0x17], fileData.begin())) {
            majorVersion = fileData[0x17];
            minorVersion = fileData[0x18];
            magicIsOk = true;
            cout << "Found CSW " << static_cast<size_t>(majorVersion)
                << "." << static_cast<size_t>(minorVersion) << endl;
        }
    }
}

void CSWFile::parse(std::vector<uint32_t> &pulseData,
        std::set<size_t> &indexData,
        std::set<size_t> &stopData) {

    size_t pointer = 0;
    size_t pulses = 0;
    rate = fileData[0x19] + 0x100 * fileData[0x1a];

    if (majorVersion == 0x01 && minorVersion == 0x01) {
        compression = fileData[0x1b];
        flags = fileData[0x1c];
        pointer = 0x20;
    } else if (majorVersion == 0x02 && minorVersion == 0x00) {
        compression = fileData[0x21];
        flags = fileData[0x22];
        cout << "CSW created with: " << &fileData[0x24] << endl;
        pointer = 0x34 + fileData[0x23];
    }

    switch (compression) {
        case 0x01:
            cout << "Using RLE compression" << endl;
            break;

        case 0x02:
            cout << "Using ZLIB+RLE compression" << endl;
            break;

        default:
            cout << "Using unknown compression" << endl;
            break;
    }

    cout << "Initial polarity: " << ((flags & 0x01) ? "HIGH" : "LOW") << endl;

    if (!pulseData.empty()) {
        indexData.insert(pulseData.size());
        stopData.insert(pulseData.size());
    }

    if (flags & 0x01) {
        pulseData.push_back(3500);
    }

    if (compression == 2) {
        size_t expectedPulses = fileData[0x1d] + 0x100 * fileData[0x1e]
            + 0x10000 * fileData[0x1f] + 0x1000000 * fileData[0x20];

        cout << "Expected pulses: " << expectedPulses << endl;

        vector<uint8_t> deflated(fileData.begin() + pointer, fileData.end());
        vector<uint8_t> inflated(5 * expectedPulses, 0x00);

        if (inflateBuffer(deflated, inflated)) {
            fileData.resize(inflated.size() + pointer);
            copy(inflated.begin(), inflated.end(), &fileData[pointer]);
        }
    }

    double pulse;
    while (pointer < fileData.size()) {
        if (fileData[pointer] == 0x00) {
            pulse = fileData[pointer + 1]
                + 0x100 * fileData[pointer + 2]
                + 0x10000 * fileData[pointer + 3]
                + 0x1000000 * fileData[pointer + 4];
            pointer += 5;
        } else {
            pulse = fileData[pointer];
            ++pointer;
        }

        ++pulses;
        pulse *= 3500000.0 / rate;
        pulseData.push_back(static_cast<uint32_t>(pulse));
    }

    cout << "Got " << pulses << " pulses." << endl;
}

// vim: et:sw=4:ts=4
