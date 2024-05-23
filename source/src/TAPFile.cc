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

#include "TAPFile.h"

void TAPFile::load(string const& fileName) {
    char c;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good()) {
        fileData.clear();
        while (ifs.get(c)) {
            fileData.push_back(c);
        }
    }

    name = fileName;
}

void TAPFile::parse(
        vector<uint32_t> &pulseData,
        set<size_t> &indexData,
        set<size_t> &stopData) {

    static uint32_t constexpr PILOT_PULSE = 2168;
    static uint32_t constexpr SYNC_PULSE_1 = 667;
    static uint32_t constexpr SYNC_PULSE_2 = 735;
    static uint32_t constexpr DATA_PULSE_0 = 855;
    static uint32_t constexpr DATA_PULSE_1 = 1710;
    static uint32_t constexpr PILOT_HEAD_LENGTH = 8064;
    static uint32_t constexpr PILOT_DATA_LENGTH = 3224;
    static uint32_t constexpr MILLISECOND_PAUSE = 3500;

    size_t dataLength;
    uint8_t flagByte;
    uint8_t byte;

    // Parse data from the beginning. Do not erase previous pulseData, so
    // multiple tapes can be concatenated.
    pointer = 0;
    if (pulseData.size()) {
        indexData.insert(pulseData.size());
        stopData.insert(pulseData.size());
    }

    while ((pointer + 2) < fileData.size()) {
        // Each block in the ROM loader has:
        // 1. A pilot tone.
        // 2. Two sync pulses.
        // 3. Data.
        // 4. A pause.

        dataLength = fileData[pointer + 1] * 0x100 + fileData[pointer];
        if (pointer + 2 + dataLength > fileData.size()) {
            cout << "Error: Missing data in TAP block. '" << name << "' may be corrupt." << endl;
            break;
        }

        flagByte = fileData[pointer + 2];

        cout << "Flag: " << static_cast<size_t>(flagByte) << "  ";
        cout << "Length: " << dataLength << endl;

        // Insert the pilot tone.
        pulseData.insert(pulseData.end(),
                (flagByte & 0x80) ? PILOT_DATA_LENGTH : PILOT_HEAD_LENGTH,
                PILOT_PULSE);

        // Insert the sync pulses.
        pulseData.push_back(SYNC_PULSE_1);
        pulseData.push_back(SYNC_PULSE_2);

        // Insert the data.
        for (size_t ii = 0; ii < dataLength; ++ii) {
            byte = fileData[pointer + 2 + ii];
            for (size_t jj = 0; jj < 8; ++jj) {
                pulseData.insert(pulseData.end(),
                        2, (byte & 0x80) ? DATA_PULSE_1 : DATA_PULSE_0);
                byte <<= 1;
            }
        }

        // Insert the pause. Annotate the pause.
        pulseData.push_back(999 * MILLISECOND_PAUSE);
        pulseData.push_back(MILLISECOND_PAUSE);
        indexData.insert(pulseData.size());

        pointer += dataLength + 2;
    }

    cout << "Got " << pulseData.size() << " pulses." << endl;
}

// vim: et:sw=4:ts=4
