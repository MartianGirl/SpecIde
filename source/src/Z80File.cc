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

#include "Z80File.h"

using namespace std;

void Z80File::load(string const& fileName) {

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

bool Z80File::parse() {

    if (!checkVersion()) {
        return false;
    }

    bool result = true;
    result = result && parseHeader();
    result = result && fillMemory();
    return result;
}

bool Z80File::checkVersion() {

    // Reset the index at which RAM data starts.
    dataIndex = 0;
    // Check if there is enough data for a V1 Z80 file.
    if (fileData.size() < 30) {
        return false;
    }

    // If yes, obtain the value of PC.
    state.pc = fileData[6] + 0x100 * fileData[7];
    // If PC == 0x0000, then it is either V2 or V3.
    if (!state.pc) {
        uint32_t extLen = fileData[30] + 0x100 * fileData[31];

        switch (extLen) {
            case 23:
                state.type = SnapType::Z80_V2;
                break;
            case 54:    // fall-through
            case 55:
                state.type = SnapType::Z80_V3;
                break;
            default:
                return false;
        }

        dataIndex = 32 + extLen;
    } else {
        state.type = SnapType::Z80_V1;
        dataIndex = 30;
    }
    return true;
}

bool Z80File::parseHeader() {

    state.af = fileData[1] + 0x100 * fileData[0];
    state.bc = fileData[2] + 0x100 * fileData[3];
    state.hl = fileData[4] + 0x100 * fileData[5];

    if (state.type == SnapType::Z80_V1) {
        state.pc = fileData[6] + 0x100 * fileData[7];
    } else {
        state.pc = fileData[32] + 0x100 * fileData[33];
    }
    
    state.sp = fileData[8] + 0x100 * fileData[9];
    state.ir = ((fileData[11] & 0x7F) | ((fileData[12] & 0x1) << 7)) + 0x100 * fileData[10];

    state.border = (fileData[12] & 0x0E) >> 1;
    state.samrom = ((fileData[12] & 0x10) == 0x10);

    state.dataIsCompressed = ((fileData[12] & 0x20) == 0x20);

    state.de = fileData[13] + 0x100 * fileData[14];
    state.bc_ = fileData[15] + 0x100 * fileData[16];
    state.de_ = fileData[17] + 0x100 * fileData[18];
    state.hl_ = fileData[19] + 0x100 * fileData[20];

    state.af_ = fileData[22] + 0x100 * fileData[21];

    state.iy = fileData[23] + 0x100 * fileData[24];
    state.ix = fileData[25] + 0x100 * fileData[26];

    state.iff = (fileData[27] ? 0x1 : 0x0) | (fileData[28] ? 0x4 : 0x0);
    state.im = fileData[29] & 0x3;
    if (state.im > 0x2) {
        cout << "Cannot load Z80 file: " << name << " provides an invalid value for IM (3)" << endl;
        return false;
    }

    state.issue2 = ((fileData[29] & 0x4) == 0x4);
    switch ((fileData[29] & 0xC0) >> 6) {
        case 0: state.joystick = JoystickType::CURSOR; break;
        case 1: state.joystick = JoystickType::KEMPSTON_OLD; break;
        default: state.joystick = JoystickType::SINCLAIR; break;
    }

    if (state.type == SnapType::Z80_V2 || state.type == SnapType::Z80_V3) {
        state.port_0x7ffd = fileData[35];

        state.emuRefresh = ((fileData[37] & 0x01) == 0x01);

        if ((fileData[37] & 0x02) == 0x00) {
            cout << "Warning: " << name << " - LDIR emulation OFF is not supported. " << endl;
        }
        state.emuAy8912 = ((fileData[37] & 0x04) == 0x04);
        state.emuFuller = state.emuAy8912 && ((fileData[37] & 0x40) == 0x40);

        bool modifyHardware = (fileData[37] & 0x80) == 0x80;

        state.port_0xfffd = fileData[38];
        for (size_t ii = 0; ii < 0x10; ++ii) {
            state.ayRegs[ii] = fileData[39 + ii];
        }

        if (state.type == SnapType::Z80_V3) {
            // Model selection, Z80 V3
            switch (fileData[34]) {
                case 0: // fall-through
                case 1: // fall-through
                case 2: // fall-through
                case 3: 
                    state.model = state.issue2 ? SnapshotModel::ZX_48K_ISSUE2 : SnapshotModel::ZX_48K_ISSUE3;
                    break;
                case 4: // fall-through
                case 5: // fall-through
                case 6: // fall-through
                    state.model = modifyHardware ? SnapshotModel::ZX_PLUS2 : SnapshotModel::ZX_128K;
                    break;
                case 7: // fall-through
                case 8:
                    state.model = modifyHardware ? SnapshotModel :: ZX_PLUS2A : SnapshotModel::ZX_PLUS3;
                    break;
                case 9:
                    state.model = SnapshotModel::PENTAGON;
                    break;
                case 12:
                    state.model = SnapshotModel::ZX_PLUS2;
                    break;
                case 13:
                    state.model = SnapshotModel::ZX_PLUS2A;
                    break;
                default:
                    break;
            }

            state.tStates = fileData[57] * 0x10000 + fileData[56] * 0x100 + fileData[55];

            if (dataIndex == 87) {
                state.port_0x1ffd = fileData[86];
            }

        } else {
            // Model selection, Z80 V2
            switch (fileData[34]) {
                case 0: // fall-through
                case 1: // fall-through
                case 2:
                    state.model = state.issue2 ? SnapshotModel::ZX_48K_ISSUE2 : SnapshotModel::ZX_48K_ISSUE3;
                    break;
                case 3: // fall-through
                case 4:
                    state.model = modifyHardware ? SnapshotModel::ZX_PLUS2 : SnapshotModel::ZX_128K;
                    break;
                case 7: // fall-through
                case 8:
                    state.model = modifyHardware ? SnapshotModel :: ZX_PLUS2A : SnapshotModel::ZX_PLUS3;
                    break;
                case 9:
                    state.model = SnapshotModel::PENTAGON;
                    break;
                case 12:
                    state.model = SnapshotModel::ZX_PLUS2;
                    break;
                case 13:
                    state.model = SnapshotModel::ZX_PLUS2A;
                    break;
                default:
                    break;
            }
        }
    } else {
        state.model = state.issue2 ? SnapshotModel::ZX_48K_ISSUE2 : SnapshotModel::ZX_48K_ISSUE3;
    }

    return true;
}

bool Z80File::fillMemory() {

    if (state.type == SnapType::Z80_V1) {
        decompressBlock(fileData.size() - dataIndex, dataIndex, state.memory[0]);
        if (state.memory[0].size() == 0xC000) {
            state.memory[5].assign(&state.memory[0][0x0000], &state.memory[0][0x4000]);
            state.memory[2].assign(&state.memory[0][0x4000], &state.memory[0][0x8000]);
            state.memory[0].assign(&state.memory[0][0x8000], &state.memory[0][0xC000]);
        } else {
            return false;
        }
    } else {
        while (static_cast<uint32_t>(dataIndex + 3) < fileData.size()) {
            size_t length = fileData[dataIndex] + 0x100 * fileData[dataIndex + 1];
            uint8_t numPage = fileData[dataIndex + 2];
            size_t page = getPage(numPage);
            dataIndex += 3;

            if (length == 0xFFFF) {
                if (dataIndex + 0x4000 <= fileData.size()) {
                    if (page != UINT8_MAX) {
                        state.memory[page].assign(&fileData[dataIndex], &fileData[dataIndex + 0x4000]);
                    }
                    dataIndex += 0x4000;
                } else {
                    cout << "Cannot load Z80 snapshot: Missing data." << endl;
                    return false;
                }
            } else {
                if (dataIndex + length <= fileData.size()) {
                    if (page != UINT8_MAX) {
                        decompressBlock(length, dataIndex, state.memory[page]);
                        if (state.memory[page].size() != 0x4000) {
                            return false;
                        }
                    }
                    dataIndex += length;
                } else {
                    cout << "Cannot load Z80 snapshot: Missing data." << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

void Z80File::decompressBlock(size_t length, size_t start, vector<uint8_t>& buffer) {

    size_t index = start;
    bool finished = false;
    buffer.clear();

    while (!finished) {
        if (fileData[index] == 0xED && fileData[index + 1] == 0xED) {
            buffer.insert(buffer.end(), fileData[index + 2], fileData[index + 3]);
            index += 4;
            length -= 4;
        } else if (state.type == SnapType::Z80_V1 && fileData[index] == 0x00 && fileData[index + 1] == 0xED
                && fileData[index + 2] == 0xED && fileData[index + 3] == 0x00) {
            finished = true;
        } else {
            buffer.push_back(fileData[index]);
            ++index;
            --length;
        }

        if (!length) {
            finished = true;
        }
    }
}

uint8_t Z80File::getPage(uint8_t numPage) {

    if (state.model == SnapshotModel::ZX_48K_ISSUE2 || state.model == SnapshotModel::ZX_48K_ISSUE3) {
        switch (numPage) {
            case 4: return 2;
            case 5: return 0;
            case 8: return 5;
            default: return UINT8_MAX;
        }
    } else {
        switch (numPage) {
            case 3: return 0;
            case 4: return 1;
            case 5: return 2;
            case 6: return 3;
            case 7: return 4;
            case 8: return 5;
            case 9: return 6;
            case 10: return 7;
            default: return UINT8_MAX;
        }
    }
}
// vim: et:sw=4:ts=4
