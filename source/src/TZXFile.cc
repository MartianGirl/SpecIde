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
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "TZXFile.h"

using namespace std;

// This one opens a TZX file, loads its contents, tries to put nextBlock
// to the beginning of the first block.
void TZXFile::load(string const& fileName) {

    name = fileName;
    char c;

    magicIsOk = false;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good()) {
        fileData.clear();
        while (ifs.get(c)) {
            fileData.push_back(c);
        }

        // TZX header is:
        // 0-6 - ZXTape!
        // 7   - 0x1A
        // 8   - Major version number (0x01)
        // 9   - Minor version number
        //
        // If the magic checks, store the version numbers.
        if (equal(&magic[0x00], &magic[0x08], fileData.begin())) {
            majorVersion = fileData[0x08];
            minorVersion = fileData[0x09];
            magicIsOk = true;
        }
    }
}

void TZXFile::parse(
        vector<uint32_t> &pulseData,
        set<size_t> &indexData,
        set<size_t> &stopData,
        set<size_t> &stopIf48K) {

    size_t dataLength;
    size_t headLength;
    uint8_t flagByte;
    uint8_t byte;
    uint8_t blockId;

    uint32_t pilotPulse;
    uint32_t syncPulse1;
    uint32_t syncPulse2;
    uint32_t dataPulse0;
    uint32_t dataPulse1;
    uint32_t pilotLength;
    uint32_t pause;
    uint32_t bitsInLastByte;
    uint32_t bitsInByte;

    uint32_t symbolsInPilot;
    uint32_t maxPilotSymLen;
    uint32_t pilotAlphabetSize;
    uint32_t symbolsInData;
    uint32_t bitsPerDataSymbol;
    uint32_t maxDataSymLen;
    uint32_t dataAlphabetSize;

    uint32_t sampleStep;

    vector<uint32_t> pilotAlphabet;
    vector<uint32_t> dataAlphabet;

    size_t tableIndex;

    string blockName;
    ss.str("");

    if (magicIsOk) {
        // Do not erase previous pulseData, so multiple tapes can be
        // concatenated.
        pointer = 0x0A;
        romData.clear();
        if (!pulseData.empty()) {
            indexData.insert(pulseData.size());
            stopData.insert(pulseData.size());
        }

        if (fileData[fileData.size() - 3] != 0x20) {
            // If there is no pause...
            // We'll add a "Pause 1000ms" block, just in case.
            fileData.push_back(0x20);
            fileData.push_back(0xE8);
            fileData.push_back(0x03);
        }
    } else {
        return;
    }

    while (pointer < fileData.size()) {
        blockId = fileData[pointer];

        headLength = getBlockHeaderLength();
        if (pointer + headLength > fileData.size()) {
            cout << "Error: Bad TZX block header. '" << name << "' may be corrupt." << endl;
            cout << "BlockId: " << setfill('0') << hex << setw(2) << static_cast<uint32_t>(blockId) << endl;
            cout << "Pointer: " << pointer << endl;
            cout << "Header Length: " << headLength << endl;
            cout << "Remaining bytes: " << fileData.size() << endl;
            break;
        }

        switch (blockId) {
            case 0x10:
                blockName = "Standard Speed Data";
                pilotPulse = 2168;
                syncPulse1 = 667;
                syncPulse2 = 735;
                dataPulse0 = 855;
                dataPulse1 = 1710;
                pause = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                dataLength = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];
                flagByte = fileData[pointer + headLength];
                pilotLength = (flagByte & 0x80) ? 3224 : 8064;

                if (pointer + headLength + dataLength > fileData.size()) {
                    cout << "Error: Missing data in TZX block. '" << name << "' may be corrupt." << endl;
                    break;
                }

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);

                // Sync pulses
                pulseData.push_back(syncPulse1);
                pulseData.push_back(syncPulse2);

                romData.push_back(dataLength & 0x00FF);
                romData.push_back((dataLength & 0xFF00) >> 8);

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii) {
                    byte = fileData[pointer + headLength + ii];
                    romData.push_back(byte);

                    for (size_t jj = 0; jj < 8; ++jj) {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                indexData.insert(pulseData.size());
                if (pause) {
                    addPause(pause, pulseData);
                }

                pointer += headLength + dataLength;
                break;

            case 0x11:
                blockName = "Turbo Speed Data";
                pilotPulse = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                syncPulse1 = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];
                syncPulse2 = fileData[pointer + 6] * 0x100
                    + fileData[pointer + 5];
                dataPulse0 = fileData[pointer + 8] * 0x100
                    + fileData[pointer + 7];
                dataPulse1 = fileData[pointer + 10] * 0x100
                    + fileData[pointer + 9];
                pilotLength = fileData[pointer + 12] * 0x100
                    + fileData[pointer + 11];
                pilotLength += pilotLength % 2;   // Pilot should be even
                bitsInLastByte = fileData[pointer + 13];
                pause = fileData[pointer + 15] * 0x100
                    + fileData[pointer + 14];
                dataLength = fileData[pointer + 18] * 0x10000
                    + fileData[pointer + 17] * 0x100
                    + fileData[pointer + 16];
                flagByte = fileData[pointer + headLength];

                if (pointer + headLength + dataLength > fileData.size()) {
                    cout << "Error: Missing data in TZX block. '" << name << "' may be corrupt." << endl;
                    break;
                }

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);

                // Sync pulses
                pulseData.push_back(syncPulse1);
                pulseData.push_back(syncPulse2);

                romData.push_back(dataLength & 0x00FF);
                romData.push_back((dataLength & 0xFF00) >> 8);

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii) {
                    byte = fileData[pointer + headLength + ii];
                    romData.push_back(byte);

                    bitsInByte = (ii == (dataLength - 1)) ? bitsInLastByte : 8;
                    for (size_t jj = 0; jj < bitsInByte; ++jj) {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                indexData.insert(pulseData.size());
                if (pause) {
                    addPause(pause, pulseData);
                }

                pointer += headLength + dataLength;
                break;

            case 0x12:
                blockName = "Pure Tone";
                pilotPulse = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pilotLength = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];

                // If this is the first segment of lead tone in a custom loader,
                // then it should contain an odd number of pulses.
                pilotLength += pilotLength % 2;   // Pilot should be even

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);
                pointer += headLength;
                break;

            case 0x13:
                blockName = "Sequence Of Pulses";
                dataLength = fileData[pointer + 1];
                for (size_t ii = 0; ii < dataLength; ++ii) {
                    uint32_t pulse =
                        fileData[pointer + (2 * ii) + 3] * 0x100 +
                        fileData[pointer + (2 * ii) + 2];
                    pulseData.push_back(pulse);
                }
                pointer += (dataLength * 2) + headLength;
                break;

            case 0x14:
                blockName = "Pure Data";
                dataPulse0 = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                dataPulse1 = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];
                bitsInLastByte = fileData[pointer + 5];
                pause = fileData[pointer + 7] * 0x100
                    + fileData[pointer + 6];
                dataLength = fileData[pointer + 10] * 0x10000
                    + fileData[pointer + 9] * 0x100
                    + fileData[pointer + 8];
                flagByte = fileData[pointer + headLength];

                if (pointer + headLength + dataLength > fileData.size()) {
                    cout << "Error: Missing data in TZX block. '" << name << "' may be corrupt." << endl;
                    break;
                }

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii) {
                    byte = fileData[pointer + headLength + ii];

                    bitsInByte = (ii == (dataLength - 1)) ? bitsInLastByte : 8;
                    for (size_t jj = 0; jj < bitsInByte; ++jj) {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                // Pause. Add index only if there is a pause.
                // (You usually don't want to start in the middle of
                // a SpeedLock block...)
                if (pause) {
                    indexData.insert(pulseData.size());
                    addPause(pause, pulseData);
                }

                pointer += headLength + dataLength;
                break;

            case 0x15:
                blockName = "Direct Recording";
                sampleStep = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pause = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];
                bitsInLastByte = fileData[pointer + 5];
                dataLength = fileData[pointer + 8] * 0x10000
                    + fileData[pointer + 7] * 0x100
                    + fileData[pointer + 6];

                // Serialize
                {
                    uint_fast32_t pulse = sampleStep;
                    uint_fast32_t level = (1 - pulseData.size() % 2) << 7;
                    for (size_t ii = 0; ii < dataLength; ++ii) {
                        byte = fileData[pointer + headLength + ii];
                        bitsInByte = (ii == (dataLength - 1)) ? bitsInLastByte : 8;
                        for (size_t jj = 0; jj < bitsInByte; ++jj) {
                            if (level != (byte & 0x80)) {
                                level ^= 0x80;
                                pulseData.push_back(pulse);
                                pulse = sampleStep;
                            } else {
                                pulse += sampleStep;
                            }
                            byte <<= 1;
                        }
                    }
                    pulseData.push_back(pulse);
                }

                // Pause. Add index only if there is a pause.
                if (pause) {
                    indexData.insert(pulseData.size());
                    addPause(pause, pulseData);
                }

                pointer += headLength + dataLength;
                break;

            case 0x16:
                blockName = "C64 ROM Type Data (Deprecated)";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += headLength + dataLength;
                break;

            case 0x17:
                blockName = "C64 Turbo Tape Data (Deprecated)";
                // Skipped for the moment.
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += headLength + dataLength;
                break;

            case 0x18:
                blockName = "CSW Recording (Not implemented yet)";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += headLength + dataLength;
                break;

            case 0x19:
                blockName = "Generalized Data";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pause = fileData[pointer + 6] * 0x100 + fileData[pointer + 5];

                loadSymbolData(pointer + 7,
                        symbolsInPilot, maxPilotSymLen, pilotAlphabetSize);
                loadSymbolData(pointer + 13,
                        symbolsInData, maxDataSymLen, dataAlphabetSize);
                bitsPerDataSymbol = ceil(log10(dataAlphabetSize) / log10(10));

                tableIndex = pointer + 19;
                if (symbolsInPilot > 0) {
                    tableIndex += loadSymbolAlphabet(tableIndex,
                            pilotAlphabetSize, maxPilotSymLen,
                            pilotAlphabet);

                    tableIndex += dumpPilotStream(tableIndex,
                            symbolsInPilot, pilotAlphabet, pulseData);
                }

                if (symbolsInData > 0) {
                    tableIndex += loadSymbolAlphabet(tableIndex,
                            dataAlphabetSize, maxDataSymLen, dataAlphabet);
                    tableIndex += dumpDataStream(tableIndex,
                            symbolsInData, bitsPerDataSymbol,
                            dataAlphabet, pulseData);
                }

                // Pause. Add index only if there is a pause.
                // (You usually don't want to start in the middle of
                // a SpeedLock block...)
                if (pause) {
                    indexData.insert(pulseData.size());
                    addPause(pause, pulseData);
                }

                pointer += dataLength + 5;
                assert(pointer == tableIndex);
                break;

            case 0x20:
                blockName = "Pause/Stop The Tape";
                pause = fileData[pointer + 2] * 0x100 + fileData[pointer + 1];

                // Pause = 0 means Stop The Tape. However, we'll insert
                // one second pause to properly end the block.
                indexData.insert(pulseData.size());
                if (pause == 0) {
                    // This pause will consist of one or two pulses, ensuring
                    // that the level is low at the end. Stop Data will also
                    // force a low level, so it must happen at the last pulse
                    // so far.
                    addPause(1000, pulseData);
                    stopData.insert(pulseData.size() - 1);
                } else {
                    addPause(pause, pulseData);
                }

                pointer += headLength;
                break;

            case 0x21:
                blockName = "Group Start";
                dataLength = fileData[pointer + 1];
                indexData.insert(pulseData.size());
                pointer += headLength + dataLength;
                break;

            case 0x22:
                blockName = "Group End";
                pointer += headLength;
                break;

            case 0x23:
                blockName = "Jump To Block (Not implemented yet)";
                pointer += headLength;
                break;

            case 0x24:
                blockName = "Loop Start";
                loopCounter = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += headLength;
                loopStart = pointer;
                break;

            case 0x25:
                blockName = "Loop End";
                --loopCounter;
                if (loopCounter) {
                    pointer = loopStart;
                } else {
                    pointer += headLength;
                }
                break;

            case 0x26:
                blockName = "Call Sequence (Not implemented yet)";
                pointer += 2 * (fileData[pointer + 2] * 0x100
                        + fileData[pointer + 1]) + 3;
                break;

            case 0x27:
                blockName = "Return From Sequence (Not implemented yet)";
                ++pointer;
                break;

            case 0x28:
                blockName = "Select block";
                pointer += fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1] + 3;
                break;

            case 0x2A:
                blockName = "Stop The Tape If In 48K Mode";
                stopIf48K.insert(pulseData.size());
                pointer += headLength;
                break;

            case 0x2B:
                blockName = "Set Signal Level";
                if (!fileData[pointer + 5])
                    pulseData.push_back(100);
                pointer += headLength;
                break;

            case 0x30:
                blockName = "Text Description";
                pointer += headLength + dumpComment();
                break;

            case 0x31:
                blockName = "Message";
                pointer += headLength + dumpMessage();
                break;

            case 0x32:
                blockName = "Archive Info";
                pointer += headLength + dumpArchiveInfo();
                break;

            case 0x33:
                blockName = "Hardware Type";
                pointer += headLength + 3 * fileData[pointer + 1];
                break;

            case 0x35:
                blockName = "Custom Info";
                dataLength = fileData[pointer + 20] * 0x1000000
                    + fileData[pointer + 19] * 0x10000
                    + fileData[pointer + 18] * 0x100
                    + fileData[pointer + 17];
                pointer += headLength + dataLength;
                break;

            case 0x5A:
                blockName = "Glue";
                pointer += headLength;
                break;

            default:
                break;
        }

        cout << "Found " << blockName << " block: " << endl;
        cout << ss.str();
        ss.str("");
        ss.clear();
    }

    cout << "Got " << pulseData.size() << " pulses." << endl;
}

size_t TZXFile::dumpArchiveInfo() {

    string tag;
    string text;
    uint32_t len;

    size_t length = fileData[pointer + 2] * 0x100 + fileData[pointer + 1];
    uint32_t numStrings = fileData[pointer + 3];
    uint32_t index = 4;

    ss << "--- Archive Info Block Start ---" << endl;
    for (size_t ii = 0; ii < numStrings; ++ii) {
        switch (fileData[pointer + index]) {
            case 0x00: tag = "Full Title: "; break;
            case 0x01: tag = "Software house/publisher: "; break;
            case 0x02: tag = "Author(s): "; break;
            case 0x03: tag = "Year of publication: "; break;
            case 0x04: tag = "Language: "; break;
            case 0x05: tag = "Game/utility type: "; break;
            case 0x06: tag = "Price: "; break;
            case 0x07: tag = "Protection scheme/loader: "; break;
            case 0x08: tag = "Origin: "; break;
            default: tag = "Comment(s): "; break;
        }

        len = fileData[pointer + index + 1];
        text.clear();
        for (size_t jj = 0; jj < len; ++jj) {
            text.push_back(static_cast<char>(fileData[pointer + index + 2 + jj]));
            if (fileData[pointer + index + 3 + jj] == 0x0D) text.push_back(0x0A);
        }

        ss << tag << text << endl;
        index += len + 2;
    }
    ss << "---- Archive Info Block End ----" << endl;
    return length;
}

size_t TZXFile::dumpComment() {

    string text;
    size_t length = fileData[pointer + 1];

    for (size_t ii = 0; ii < length; ++ii) {
        text.push_back(static_cast<char>(fileData[pointer + 2 + ii]));
        if (fileData[pointer + 3 + ii] == 0x0D) {
            text.push_back(0x0A);
        }
    }
    ss << "------ Comment Block Start -----" << endl << text << endl;
    ss << "------- Comment Block End ------" << endl;
    return length;
}

size_t TZXFile::dumpMessage() {

    string text;
    // size_t time = fileData[pointer + 1];
    size_t length = fileData[pointer + 2];

    for (size_t ii = 0; ii < length; ++ii) {
        text.push_back(static_cast<char>(fileData[pointer + 3 + ii]));
        if (fileData[pointer + 3 + ii] == 0x0D) text.push_back(0x0A);
    }
    ss << "------ Message Block Start -----" << endl << text << endl;
    ss << "------- Message Block End ------" << endl;

    return length;
}

void TZXFile::loadSymbolData(size_t base,
        uint32_t& numSym, uint32_t& maxLen, uint32_t& alphaSize) {

    numSym = fileData[base + 3] * 0x1000000
        + fileData[base + 2] * 0x10000
        + fileData[base + 1] * 0x100
        + fileData[base];
    maxLen = fileData[base + 4];
    alphaSize = fileData[base + 5];
}

size_t TZXFile::loadSymbolAlphabet(size_t base, uint32_t numSym, uint32_t maxLen,
        vector<uint32_t>& alphabet) {

    size_t index = base;
    uint32_t size = maxLen + 1;
    alphabet.assign(numSym * size + 1, 0);   // Symbol Size, n * (Symbol type, pulses)
    alphabet[0] = size;
    for (size_t i = 0; i < numSym; ++i) {
        ss << "Symbol: " << i << "  ";
        alphabet[i * size + 1] = fileData[index];
        ++index;
        ss << "Type: " << alphabet[i * size + 1] << "  Values:";
        for (size_t j = 0; j < maxLen; ++j) {
            alphabet[i * size + j + 2] =
                fileData[index + 2 * j + 1] * 0x100 + fileData[index + 2 * j];
            ss << " " << alphabet[i * size + j + 2];
        }
        index += 2 * maxLen;
        ss << endl;
    }

    return index - base;
}

size_t TZXFile::dumpPilotStream(size_t base, uint32_t numSym,
        vector<uint32_t> const& alphabet, vector<uint32_t>& data) {

    uint32_t rep;
    uint32_t sym;
    size_t index = base;

    ss << "Pilot:";
    for (size_t i = 0; i < numSym; ++i) {
        sym = fileData[index];
        rep = fileData[index + 2] * 0x100 + fileData[index + 1];
        index += 3;
        ss << " [" << sym << " * " << rep << "]";

        pushSymbol(rep, sym, alphabet, data);
    }
    ss << endl;

    return index - base;
}

size_t TZXFile::dumpDataStream(size_t base, uint32_t numSym, uint32_t bps,
        vector<uint32_t> const& alphabet, vector<uint32_t>& data) {

    uint32_t bit = 0;
    uint32_t mask = (1 << bps) - 1;
    uint32_t sym;
    size_t index = base;

    ss << "Data: " << numSym << " " << bps << "-bit symbols ";
    for (size_t i = 0; i < numSym; ++i) {
        index = base + (bit >> 3);
        sym = (fileData[index] >> (7 - (bit % 8))) & mask;
        bit += bps;

        pushSymbol(1, sym, alphabet, data);
    }
    ss << endl;

    return index - base;
}

void TZXFile::pushSymbol(uint32_t rep, uint32_t sym,
        vector<uint32_t> const& alphabet, vector<uint32_t>& data) {

    uint32_t size = alphabet[0];
    uint32_t type = alphabet[sym * size + 1];
    uint32_t const* first = &alphabet[sym * size + 2];
    uint32_t const* last = &alphabet[(sym + 1) * size + 1];
    last = find(first, last, 0);

    bool concatenate = false;
    switch (type) {
        case 0x00:  // Edge
            concatenate = false;
            break;

        case 0x01:  // Continue
            concatenate = true;
            break;

        case 0x02:  // Force low -- fallthrough
        case 0x03:  // Force high
            concatenate = (data.size() % 2) != (type % 2);
            break;
        default:
            assert(false);
    }

    if (concatenate) {
        data.back() += *first;
        data.insert(data.end(), first + 1, last);
    }
    for (uint32_t ii = concatenate; ii < rep; ++ii) {
        data.insert(data.end(), first, last);
    }
}

void TZXFile::addPause(uint32_t pause, vector<uint32_t>& data) {

    data.push_back(3495 * pause);
    data.push_back(5 * pause);  // Keep number of pulses even
}

size_t TZXFile::getBlockHeaderLength() {

    switch (fileData[pointer]) {
        case 0x10: return 0x05; break;
        case 0x11: return 0x13; break;
        case 0x12: return 0x05; break;
        case 0x13: return 0x02; break;
        case 0x14: return 0x0B; break;
        case 0x15: return 0x09; break;
        case 0x16: return 0x05; break;
        case 0x17: return 0x05; break;
        case 0x18: return 0x05; break;
        case 0x19: return 0x13; break;
        case 0x20: return 0x03; break;
        case 0x21: return 0x02; break;
        case 0x22: return 0x01; break;
        case 0x23: return 0x03; break;
        case 0x24: return 0x03; break;
        case 0x25: return 0x01; break;
        case 0x26: return 0x03; break;
        case 0x27: return 0x01; break;
        case 0x28: return 0x04; break;
        case 0x2A: return 0x05; break;
        case 0x2B: return 0x06; break;
        case 0x30: return 0x02; break;
        case 0x31: return 0x03; break;
        case 0x32: return 0x03; break;
        case 0x33: return 0x02; break;
        case 0x35: return 0x15; break;
        case 0x5A: return 0x0A; break;
        default: return fileData.size() + 1;
    }
}
// vim: et:sw=4:ts=4
