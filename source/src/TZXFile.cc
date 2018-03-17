#include "TZXFile.h"

// This one opens a TZX file, loads its contents, tries to put nextBlock
// to the beginning of the first block.
void TZXFile::load(string const& fileName)
{
    char c;

    magicIsOk = false;
    firstLeadTone = false;

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

    string blockName;

    if (magicIsOk)
    {
        pointer = 0x0A;
        // pulseData.clear();
        // indexData.clear();
        // stopData.clear();
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
                pilotLength = (flagByte & 0x80) ? 3223 : 8063;

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

                // Pause. Add index always.
                if (pause)
                {
                    indexData.insert(pulseData.size());
                    if ((pulseData.size() % 2) == 0)
                    {
                        pulseData.push_back(3500);
                        pulseData.push_back(3500 * (pause - 1));
                    }
                    else
                    {
                        pulseData.push_back(3500 * pause);
                    }
                }

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
                pilotLength += (pilotLength % 2) ? 0 : 1;   // Pilot should be odd
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
                    // byte &= (0xFF << (8 - bitsInByte));
                    for (size_t jj = 0; jj < bitsInByte; ++jj)
                    {
                        pulseData.insert(pulseData.end(),
                                2, (byte & 0x80) ? dataPulse1 : dataPulse0);
                        byte <<= 1;
                    }
                }

                // Pause. Add index always.
                if (pause)
                {
                    indexData.insert(pulseData.size());
                    if ((pulseData.size() % 2) == 0)
                    {
                        pulseData.push_back(3500);
                        pulseData.push_back(3500 * (pause - 1));
                    }
                    else
                    {
                        pulseData.push_back(3500 * pause);
                    }
                }

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
                if (firstLeadTone)
                    pilotLength += (pilotLength % 2) ? 1 : 0;   // Pilot should be even

                // Pilot tone
                pulseData.insert(pulseData.end(), pilotLength, pilotPulse);
                pointer += 5;
                firstLeadTone = false;
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
                    // byte &= (0xFF << (8 . bitsInByte));
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
                    if ((pulseData.size() % 2) == 0)
                    {
                        pulseData.push_back(3500);
                        pulseData.push_back(3500 * (pause - 1));
                    }
                    else
                    {
                        pulseData.push_back(3500 * pause);
                    }
                }

                pointer += dataLength + 11;
                break;

            case 0x15:
                blockName = "Direct Recording";
                // Skipped for the moment.
                dataLength = fileData[pointer + 8] * 0x10000
                    + fileData[pointer + 7] * 0x100
                    + fileData[pointer + 6];
                pointer += dataLength + 9;
                break;

            case 0x16:
            case 0x17:
                blockName = "C64 Data";
                // Skipped for the moment.
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += dataLength + 1;
                break;

            case 0x18:
            case 0x19:
                blockName = "CSW/Generalized Data";
                dataLength = fileData[pointer + 4] * 0x1000000
                    + fileData[pointer + 3] * 0x10000
                    + fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += dataLength + 5;
                break;

            case 0x20:
                blockName = "Pause/Stop The Tape";
                pause = fileData[pointer + 2] * 0x100 + fileData[pointer + 1];

                // Pause = 0 means Stop The Tape. However, we'll insert
                // one second pause to properly end the block.
                indexData.insert(pulseData.size());
                {
                    size_t delay = (pause != 0) ? pause : 1000;
                    if ((pulseData.size() % 2) == 0)
                    {
                        pulseData.push_back(3500);
                        pulseData.push_back(3500 * (delay - 1));
                    }
                    else
                    {
                        pulseData.push_back(3500 * delay);
                    }
                }

                if (pause == 0)
                    stopData.insert(pulseData.size());

                pointer += 3;
                break;

            case 0x21:
                blockName = "Group Start";
                dataLength = fileData[pointer + 1];
                indexData.insert(pulseData.size());
                pointer += dataLength + 2;
                firstLeadTone = true;
                break;

            case 0x22:
                blockName = "Group End";
                ++pointer;
                break;

            case 0x24:
                blockName = "Loop Start";
                loopCounter = fileData[pointer + 2] * 0x100
                    + fileData[pointer + 1];
                pointer += 3;
                loopStart = pointer;
                firstLeadTone = true;
                break;

            case 0x25:
                blockName = "Loop End";
                --loopCounter;
                if (loopCounter)
                    pointer = loopStart;
                else
                    pointer += 1;
                break;

            case 0x2A:
                blockName = "Stop The Tape If In 48K Mode";
                stopIf48K.insert(pulseData.size());
                pointer += 5;
                break;

            case 0x30:
                blockName = "Text Description";
                pointer += dumpComment() + 2;
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

    cout << "--- Archive info block ---" << endl;
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
        for (size_t jj = 0; jj < len; ++jj)
            text.push_back(static_cast<char>(fileData[pointer + index + 2 + jj]));

        cout << tag << text << endl;
        index += len + 2;
    }
    return length;
}

size_t TZXFile::dumpComment()
{
    string text;
    size_t length = fileData[pointer + 1];

    for (size_t i = 0; i < length; ++i)
        text.push_back(static_cast<char>(fileData[pointer + 2 + i]));
    cout << "Comment: " << text << endl;

    return length;
}

// vim: et:sw=4:ts=4
