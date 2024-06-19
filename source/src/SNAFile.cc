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

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "SNAFile.h"

using namespace std;

void SNAFile::load(string const& fileName) {

    name = fileName;
    char c;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good()) {
        fileData.clear();
        while (ifs.get(c)) {
            fileData.push_back(c);
        }
    }
}

bool SNAFile::parse() {

    bool result = true;
    result = result && parseHeader();
    result = result && fillMemory();
    return result;
}

bool SNAFile::parseHeader() {

    if (fileData.size() == 49179) {
        state.type = SnapType::SNA_48;
        state.model = SnapshotModel:: ZX_48K_ISSUE3;
    } else if (fileData.size() == 131103 || fileData.size() == 147487) {
        state.type = SnapType::SNA_128;
        state.model = SnapshotModel::PENTAGON;
    } else {
        return false;
    }

    state.ir = fileData[0] * 0x100;

    state.hl_ = fileData[1] + 0x100 * fileData[2];
    state.de_ = fileData[3] + 0x100 * fileData[4];
    state.bc_ = fileData[5] + 0x100 * fileData[6];
    state.af_ = fileData[7] + 0x100 * fileData[8];

    state.hl = fileData[9] + 0x100 * fileData[10];
    state.de = fileData[11] + 0x100 * fileData[12];
    state.bc = fileData[13] + 0x100 * fileData[14];
    state.iy = fileData[15] + 0x100 * fileData[16];
    state.ix = fileData[17] + 0x100 * fileData[18];

    state.iff = fileData[19] & 0x04;

    state.ir |= fileData[20];

    state.af = fileData[21] + 0x100 * fileData[22];
    state.sp = fileData[23] + 0x100 * fileData[24];

    state.im = fileData[25];
    if (state.im > 0x2) {
        cout << "Cannot load SNA file: " << name << " provides an invalid value for IM (3)" << endl;
        return false;
    }
    state.border = fileData[26] & 0x7;

    if (state.type == SnapType::SNA_128) {
        state.pc = fileData[49179] + 0x100 * fileData[49180];
        state.port_0x7ffd = fileData[49181];
        state.trdos = (fileData[49182] != 0);
    }

    return true;
}

bool SNAFile::fillMemory() {

    uint32_t membase = 27;
    uint32_t p = state.port_0x7ffd & 0x7;

    state.memory[5].assign(&fileData[membase + 0x0000], &fileData[membase + 0x4000]);
    state.memory[2].assign(&fileData[membase + 0x4000], &fileData[membase + 0x8000]);
    state.memory[p].assign(&fileData[membase + 0x8000], &fileData[membase + 0xC000]);

    if (state.type == SnapType::SNA_128) {
        membase = 49183;
        for (size_t ii = 0; ii < 8; ++ii) {
            if (ii != 5 && ii != 2 && ii != p) {
                state.memory[ii].assign(&fileData[membase], &fileData[membase + 0x4000]);
                membase += 0x4000;
            }
        }
    }

    return true;
}

// vim: et:sw=4:ts=4
