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
void TZXFile::load(string const& fileName)
{
    char c;

    magicIsOk = false;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good())
    {
        fileData.clear();
        while (ifs.get(c))
            fileData.push_back(c);

        // TZX header is:
        // 0-6 - ZXTape!
        // 7   - 0x1A
        // 8   - Major version number (0x01)
        // 9   - Minor version number
        //
        // If the magic checks, store the version numbers.
        if (equal(&magic[0x00], &magic[0x08], fileData.begin()))
        {
            majorVersion = fileData[0x08];
            minorVersion = fileData[0x09];
            magicIsOk = true;
        }
    }
}

void TZXFile::parse(
        vector<size_t> &pulseData,
        set<size_t> &indexData,
        set<size_t> &stopData,
        set<size_t> &stopIf48K)
{

    size_t dataLength;
    uint8_t flagByte;
    uint8_t byte;
    uint8_t blockId;

    size_t pilotPulse;
    size_t syncPulse1;
    size_t syncPulse2;
    size_t dataPulse0;
    size_t dataPulse1;
    size_t pilotLength;
    size_t pause;
    size_t bitsInLastByte;
    size_t bitsInByte;

    size_t symbolsInPilot;
    size_t maxPilotSymLen;
    size_t pilotAlphabetSize;
    size_t symbolsInData;
    size_t bitsPerDataSymbol;
    size_t maxDataSymLen;
    size_t dataAlphabetSize;

    vector<size_t> pilotAlphabet;
    vector<size_t> dataAlphabet;

    size_t tableIndex;

    string blockName;
    ss.str("");

    if (magicIsOk)
    {
        // Do not erase previous pulseData, so multiple tapes can be
        // concatenated.
        pointer = 0x0A;
        romData.clear();
        if (pulseData.size() != 0)
        {
            indexData.insert(pulseData.size());
            stopData.insert(pulseData.size());
        }

        if (fileData[fileData.size() - 3] != 0x20)
        {
            // If there is no pause...
            // We'll add a "Pause 1000ms" block, just in case.
            fileData.push_back(0x20);
            fileData.push_back(0xE8);
            fileData.push_back(0x03);
        }
    }
    else
        return;

    while (pointer < fileData.size())
    {
        blockId = fileData[pointer];
        switch (blockId)
        {
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
                flagByte = fileData[pointer + 5];
                pilotLength = (flagByte & 0x80) ? 3224 : 8064;

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);

                // Sync pulses
                pulseData.push_back(syncPulse1);
                pulseData.push_back(syncPulse2);

                romData.push_back(dataLength & 0x00FF);
                romData.push_back((dataLength & 0xFF00) >> 8);

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii)
                {
                    byte = fileData[pointer + 5 + ii];
                    romData.push_back(byte);

                    for (size_t jj = 0; jj < 8; ++jj)
                    {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                indexData.insert(pulseData.size());
                if (pause)
                    addPause(pause, pulseData);

                pointer += dataLength + 5;
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
                pilotLength += (pilotLength % 2) ? 1 : 0;   // Pilot should be even
                bitsInLastByte = fileData[pointer + 13];
                pause = fileData[pointer + 15] * 0x100
                    + fileData[pointer + 14];
                dataLength = fileData[pointer + 18] * 0x10000
                    + fileData[pointer + 17] * 0x100
                    + fileData[pointer + 16];
                flagByte = fileData[pointer + 19];

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);

                // Sync pulses
                pulseData.push_back(syncPulse1);
                pulseData.push_back(syncPulse2);

                romData.push_back(dataLength & 0x00FF);
                romData.push_back((dataLength & 0xFF00) >> 8);

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii)
                {
                    byte = fileData[pointer + 19 + ii];
                    romData.push_back(byte);

                    bitsInByte = (ii == (dataLength - 1)) ? bitsInLastByte : 8;
                    for (size_t jj = 0; jj < bitsInByte; ++jj)
                    {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                indexData.insert(pulseData.size());
                if (pause)
                    addPause(pause, pulseData);

                pointer += dataLength + 19;
                break;

            case 0x12:
                blockName = "Pure Tone";
                pilotPulse = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pilotLength = fileData[pointer + 4] * 0x100
                    + fileData[pointer + 3];

                // If this is the first segment of lead tone in a custom loader,
                // then it should contain an odd number of pulses.
                pilotLength += (pilotLength % 2) ? 1 : 0;   // Pilot should be even

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);
                pointer += 5;
                break;

            case 0x13:
                blockName = "Sequence Of Pulses";
                dataLength = fileData[pointer + 1];
                for (size_t ii = 0; ii < dataLength; ++ii)
                {
                    size_t pulse =
                        fileData[pointer + (2 * ii) + 3] * 0x100 +
                        fileData[pointer + (2 * ii) + 2];
                    pulseData.push_back(pulse);
                }
                pointer += (dataLength * 2) + 2;
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
                flagByte = fileData[pointer + 11];

                // Data
                for (size_t ii = 0; ii < dataLength; ++ii)
                {
                    byte = fileData[pointer + 11 + ii];

                    bitsInByte = (ii == (dataLength - 1)) ? bitsInLastByte : 8;
                    for (size_t jj = 0; jj < bitsInByte; ++jj)
                    {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                // Pause. Add index only if there is a pause.
                // (You usually don't want to start in the middle of
                // a SpeedLock block...)
                if (pause)
                {
                    indexData.insert(pulseData.size());
                    addPause(pause, pulseData);
                }

                pointer += dataLength + 11;
                break;

            case 0x15:
                blockName = "Direct Recording (Not implemented yet)";
                // Skipped for the moment.
                dataLength = fileData[pointer + 8] * 0x10000
                    + fileData[pointer + 7] * 0x100
                    + fileData[pointer + 6];
                pointer += dataLength + 9;
                break;

            case 0x16:
                blockName = "C64 ROM Type Data (Deprecated)";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += dataLength + 1;
                break;

            case 0x17:
                blockName = "C64 Turbo Tape Data (Deprecated)";
                // Skipped for the moment.
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += dataLength + 1;
                break;

            case 0x18:
                blockName = "CSW Recording (Not implemented yet)";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pause = fileData[pointer + 6] * 0x100 + fileData[pointer + 5];
                pointer += dataLength + 5;
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
                if (symbolsInPilot > 0)
                {
                    tableIndex += loadSymbolAlphabet(tableIndex,
                            pilotAlphabetSize, maxPilotSymLen,
                            pilotAlphabet);

                    tableIndex += dumpPilotStream(tableIndex,
                            symbolsInPilot, pilotAlphabet, pulseData);
                }

                if (symbolsInData > 0)
                {
                    tableIndex += loadSymbolAlphabet(tableIndex,
                            dataAlphabetSize, maxDataSymLen, dataAlphabet);
                    tableIndex += dumpDataStream(tableIndex,
                            symbolsInData, bitsPerDataSymbol,
                            dataAlphabet, pulseData);
                }

                // Pause. Add index only if there is a pause.
                // (You usually don't want to start in the middle of
                // a SpeedLock block...)
                if (pause)
                {
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
                if (pause == 0)
                {
                    // This pause will consist of one or two pulses, ensuring
                    // that the level is low at the end. Stop Data will also
                    // force a low level, so it must happen at the last pulse
                    // so far.
                    addPause(1000, pulseData);
                    stopData.insert(pulseData.size() - 1);
                }
                else
                    addPause(pause, pulseData);

                pointer += 3;
                break;

            case 0x21:
                blockName = "Group Start";
                dataLength = fileData[pointer + 1];
                indexData.insert(pulseData.size());
                pointer += dataLength + 2;
                break;

            case 0x22:
                blockName = "Group End";
                ++pointer;
                break;

            case 0x23:
                blockName = "Jump To Block (Not implemented yet)";
                pointer += 3;
                break;

            case 0x24:
                blockName = "Loop Start";
                loopCounter = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += 3;
                loopStart = pointer;
                break;

            case 0x25:
                blockName = "Loop End";
                --loopCounter;
                if (loopCounter)
                    pointer = loopStart;
                else
                    pointer += 1;
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
                pointer += 5;
                break;

            case 0x2B:
                blockName = "Set Signal Level";
                if (!fileData[pointer + 5])
                    pulseData.push_back(100);
                pointer += 6;
                break;

            case 0x30:
                blockName = "Text Description";
                pointer += dumpComment() + 2;
                break;

            case 0x31:
                blockName = "Message";
                pointer += dumpMessage() + 2;
                break;

            case 0x32:
                blockName = "Archive Info";
                pointer += dumpArchiveInfo() + 3;
                break;

            case 0x33:
                blockName = "Hardware Type";
                pointer += 3 * fileData[pointer + 1] + 2;
                break;

            case 0x35:
                blockName = "Custom Info";
                dataLength = fileData[pointer + 20] * 0x1000000
                    + fileData[pointer + 19] * 0x10000
                    + fileData[pointer + 18] * 0x100
                    + fileData[pointer + 17];
                pointer += dataLength + 21;
                break;

            case 0x5A:
                blockName = "Glue";
                pointer += 10;
                break;

            default:
                break;
        }

        cout << "Found " << blockName << " block." << endl;
        cout << ss.str();
        ss.str("");
        ss.clear();
    }

    cout << "Got " << pulseData.size() << " pulses." << endl;
}

size_t TZXFile::dumpArchiveInfo()
{
    string tag;
    string text;
    size_t len;

    size_t length = fileData[pointer + 2] * 0x100 + fileData[pointer + 1];
    size_t numStrings = fileData[pointer + 3];
    size_t index = 4;

    ss << "--- Archive info block ---" << endl;
    for (size_t ii = 0; ii < numStrings; ++ii)
    {
        switch (fileData[pointer + index])
        {
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
    ss << "--------------------------" << endl;
    return length;
}

size_t TZXFile::dumpComment()
{
    string text;
    size_t length = fileData[pointer + 1];

    for (size_t ii = 0; ii < length; ++ii) {
        text.push_back(static_cast<char>(fileData[pointer + 2 + ii]));
        if (fileData[pointer + 3 + ii] == 0x0D) text.push_back(0x0A);
    }
    ss << "--- Comment block ---" << endl << text << endl;
    ss << "---------------------" << endl;
    return length;
}

size_t TZXFile::dumpMessage()
{
    string text;
    // size_t time = fileData[pointer + 1];
    size_t length = fileData[pointer + 2];

    for (size_t ii = 0; ii < length; ++ii) {
        text.push_back(static_cast<char>(fileData[pointer + 3 + ii]));
        if (fileData[pointer + 3 + ii] == 0x0D) text.push_back(0x0A);
    }
    ss << "--- Message block ---" << endl << text << endl;
    ss << "---------------------" << endl;

    return length;
}

void TZXFile::loadSymbolData(size_t base,
        size_t& numSym, size_t& maxLen, size_t& alphaSize)
{
    numSym = fileData[base + 3] * 0x1000000
        + fileData[base + 2] * 0x10000
        + fileData[base + 1] * 0x100
        + fileData[base];
    maxLen = fileData[base + 4];
    alphaSize = fileData[base + 5];
}

size_t TZXFile::loadSymbolAlphabet(size_t base, size_t numSym, size_t maxLen,
        vector<size_t>& alphabet)
{
    size_t index = base;
    size_t size = maxLen + 1;
    alphabet.assign(numSym * size + 1, 0);   // Symbol Size, n * (Symbol type, pulses)
    alphabet[0] = size;
    for (size_t i = 0; i < numSym; ++i)
    {
        ss << "Symbol: " << i << "  ";
        alphabet[i * size + 1] = fileData[index];
        ++index;
        ss << "Type: " << alphabet[i * size + 1] << "  Values:";
        for (size_t j = 0; j < maxLen; ++j)
        {
            alphabet[i * size + j + 2] =
                fileData[index + 2 * j + 1] * 0x100 + fileData[index + 2 * j];
            ss << " " << alphabet[i * size + j + 2];
        }
        index += 2 * maxLen;
        ss << endl;
    }

    return index - base;
}

size_t TZXFile::dumpPilotStream(size_t base, size_t numSym,
        vector<size_t> const& alphabet, vector<size_t>& data)
{
    size_t rep;
    size_t sym;
    size_t index = base;

    ss << "Pilot:";
    for (size_t i = 0; i < numSym; ++i)
    {
        sym = fileData[index];
        rep = fileData[index + 2] * 0x100 + fileData[index + 1];
        index += 3;
        ss << " [" << sym << " * " << rep << "]";

        pushSymbol(rep, sym, alphabet, data);
    }
    ss << endl;

    return index - base;
}

size_t TZXFile::dumpDataStream(size_t base, size_t numSym, size_t bps,
        vector<size_t> const& alphabet, vector<size_t>& data)
{
    size_t bit = 0;
    size_t mask = (1 << bps) - 1;
    size_t index = base;
    size_t sym;

    ss << "Data: " << numSym << " " << bps << "-bit symbols ";
    for (size_t i = 0; i < numSym; ++i)
    {
        index = base + (bit >> 3);
        sym = (fileData[index] >> (7 - (bit % 8))) & mask;
        bit += bps;

        pushSymbol(1, sym, alphabet, data);
    }
    ss << endl;

    return index - base;
}

void TZXFile::pushSymbol(size_t rep, size_t sym,
        vector<size_t> const& alphabet, vector<size_t>& data)
{
    size_t size = alphabet[0];
    size_t type = alphabet[sym * size + 1];
    size_t const* first = &alphabet[sym * size + 2];
    size_t const* last = &alphabet[(sym + 1) * size + 1];
    last = find(first, last, 0);

    switch (type)
    {
        case 0x00:  // Edge
            for (size_t i = 0; i < rep; ++i)
                data.insert(data.end(), first, last);
            break;

        case 0x01:  // Continue
            for (size_t i = 0; i < rep; ++i)
            {
                data.back() += *first;
                data.insert(data.end(), first + 1, last);
            }
            break;
        case 0x02:  // Force low
        case 0x03:  // Force high
            if ((data.size() % 2) == (type % 2))
            {
                data.insert(data.end(), first, last);
            }
            else
            {
                data.back() += *first;
                data.insert(data.end(), first + 1, last);
            }
            break;
        default:
            assert(false);
    }
}

void TZXFile::addPause(size_t pause, vector<size_t>& data)
{
    data.push_back(3500 * pause);
    data.push_back(100);  // Keep number of pulses even
}

// vim: et:sw=4:ts=4
