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
    result = result && parseStatus();
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
    status.pc = fileData[6] + 0x100 * fileData[7];
    // If PC == 0x0000, then it is either V2 or V3.
    if (!status.pc) {
        uint16_t extLen = fileData[30] + 0x100 * fileData[31];

        switch (extLen) {
            case 23:
                version = 2;
                break;
            case 54:    // fall-through
            case 55:
                version = 3;
                break;
            default:
                return false;
        }

        dataIndex = 30 + extLen;
    } else {
        version = 1;
        dataIndex = 30;
    }
    return true;
}

bool Z80File::parseStatus() {

    status.af = fileData[1] + 0x100 * fileData[0];
    status.bc = fileData[2] + 0x100 * fileData[3];
    status.hl = fileData[4] + 0x100 * fileData[5];

    if (version == 1) {
        status.pc = fileData[6] + 0x100 * fileData[7];
    } else {
        status.pc = fileData[32] + 0x100 * fileData[33];
    }
    
    status.sp = fileData[8] + 0x100 * fileData[9];
    status.ir = ((fileData[11] & 0x7F) | ((fileData[12] & 0x1) << 7)) + 0x100 * fileData[10];

    status.border = (fileData[12] & 0x0E) >> 1;
    if (fileData[12] & 0x10) {
        cout << "Cannot load Z80 file: SamRom not supported. Aborting." << endl;
        return false;
    } 
    status.dataIsCompressed = ((fileData[12] & 0x20) == 0x20);

    status.de = fileData[13] + 0x100 * fileData[14];
    status.bc_ = fileData[15] + 0x100 * fileData[16];
    status.de_ = fileData[17] + 0x100 * fileData[18];
    status.hl_ = fileData[19] + 0x100 * fileData[20];

    status.af_ = fileData[22] + 0x100 * fileData[21];

    status.iy = fileData[23] + 0x100 * fileData[24];
    status.ix = fileData[25] + 0x100 * fileData[26];

    status.iff = (fileData[27] ? 0x1 : 0x0) | (fileData[28] ? 0x2 : 0x0);

    status.im = fileData[29] & 0x3;
    if (status.im == 0x3) {
        cout << "Cannot load Z80 file: " << name << " attempts to set IM = 3" << endl;
        return false;
    }
    status.issue2 = ((fileData[29] & 0x4) == 0x4);
    status.joystickType = (fileData[29] & 0xC0) >> 6;

    if (version > 1) {
        status.port_0x7ffd = fileData[35];

        if ((fileData[37] & 0x01) == 0x00) {
            cout << "Warning: " << name << " - R register emulation OFF is not supported. " << endl;
        }
        if ((fileData[37] & 0x02) == 0x00) {
            cout << "Warning: " << name << " - LDIR emulation OFF is not supported. " << endl;
        }
        status.emuAy8912 = ((fileData[37] & 0x04) == 0x04);
        status.emuFuller = status.emuAy8912 && ((fileData[37] & 0x40) == 0x40);

        bool modifyHardware = (fileData[37] & 0x80) == 0x80;

        status.port_0xfffd = fileData[38];
        for (size_t ii = 0; ii < 0x10; ++ii) {
            status.ayRegs[ii] = fileData[39 + ii];
        }

        if (version > 2) {
            // Model selection, Z80 V3
            switch (fileData[34]) {
                case 0: // fall-through
                case 1: // fall-through
                case 3: 

                    if (status.issue2) {
                        status.model = SnapshotModel::ZX_48K_ISSUE2;
                    } else {
                        status.model = SnapshotModel::ZX_48K_ISSUE3;
                    }
                    break;

                case 4: // fall-through
                case 5: // fall-through
                case 6: // fall-through
                    if (modifyHardware) {
                        status.model = SnapshotModel::ZX_PLUS2;
                    } else {
                        status.model = SnapshotModel::ZX_128K;
                    }
                    break;

                case 7: // fall-through
                case 8:
                    if (modifyHardware) {
                        status.model = SnapshotModel::ZX_PLUS2A;
                    } else {
                        status.model = SnapshotModel::ZX_PLUS3;
                    }
                    break;

                case 9:
                    status.model = SnapshotModel::PENTAGON;
                    break;

                case 12:
                    status.model = SnapshotModel::ZX_PLUS2;
                    break;

                case 13:
                    status.model = SnapshotModel::ZX_PLUS2A;
                    break;

                default:
                    break;
            }
        } else {
            // Model selection, Z80 V2
            switch (fileData[34]) {
                case 0: // fall-through
                case 1:
                    if (status.issue2) {
                        status.model = SnapshotModel::ZX_48K_ISSUE2;
                    } else {
                        status.model = SnapshotModel::ZX_48K_ISSUE3;
                    }
                    break;

                case 3: // fall-through
                case 4:
                    if (modifyHardware) {
                        status.model = SnapshotModel::ZX_PLUS2;
                    } else {
                        status.model = SnapshotModel::ZX_128K;
                    }
                    break;

                case 7: // fall-through
                case 8:
                    if (modifyHardware) {
                        status.model = SnapshotModel::ZX_PLUS2A;
                    } else {
                        status.model = SnapshotModel::ZX_PLUS3;
                    }
                    break;

                case 9:
                    status.model = SnapshotModel::PENTAGON;
                    break;

                case 12:
                    status.model = SnapshotModel::ZX_PLUS2;
                    break;

                case 13:
                    status.model = SnapshotModel::ZX_PLUS2A;
                    break;

                default:
                    break;
            }

            if (dataIndex == 55) {
                status.port_0x1ffd = fileData[86];
            }
        }
    }

    return true;
}

bool Z80File::fillMemory() {

    vector<uint8_t> memory;

    if (version == 1) {
        decompressBlock(fileData.size() - dataIndex, dataIndex, memory);
        if (memory.size() != 0xC000) {
            return false;
        }
        std::copy(status.ramPages[5], &memory[0x0000], &memory[0x4000]);
        std::copy(status.ramPages[2], &memory[0x4000], &memory[0x8000]);
        std::copy(status.ramPages[0], &memory[0x8000], &memory[0xC000]);
    } else {
        while (static_cast<uint32_t>(dataIndex + 3) < fileData.size()) {
            size_t length = fileData[dataIndex] + 0x100 * fileData[dataIndex + 1];
            uint8_t numPage = fileData[dataIndex + 2];
            dataIndex += 3;
            if (dataIndex + length <= fileData.size()) {
                if (length == 0xFFFF) {
                    copy(status.ramPages[getPage(numPage)], &fileData[dataIndex], &fileData[dataIndex + 0x4000]);
                    dataIndex += 0x4000;
                } else {
                    decompressBlock(length, dataIndex, memory);
                    if (memory.size() != 0x4000) {
                        return false;
                    }
                    copy(status.ramPages[getPage(numPage)], &memory[0x0000], &memory[0x4000]);
                    dataIndex += length;
                }
            }
        }
    }
    return true;
}

void Z80File::decompressBlock(size_t length, size_t start, vector<uint8_t>& buffer) {

    size_t index = start;
    bool finished = false;

    while (!finished) {
        if (fileData[index] == 0xED && fileData[index + 1] == 0xED) {
            buffer.insert(buffer.end(), fileData[index + 2], fileData[index + 3]);
            index += 4;
            length -= 4;
        } else if (version == 1  && fileData[index] == 0x00 && fileData[index + 1] == 0xED
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

    if (status.model == SnapshotModel::ZX_48K_ISSUE2 || status.model == SnapshotModel::ZX_48K_ISSUE3) {
        switch (numPage) {
            case 4: return 2;
            case 5: return 0;
            case 8: return 5;
            default: return 0;
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
            default: return 0;
        }
    }
}
// vim: et:sw=4:ts=4
