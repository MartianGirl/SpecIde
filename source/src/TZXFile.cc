#include "TZXFile.h"

TZXFile::TZXFile() :
    magic { 'Z', 'X', 'T', 'a', 'p', 'e', '!', 0x1A },
    isTap(false),
    pointer(0), nextBlock(0),
    pulseLength(0), numPulses(0), numSamples(0),
    numBit(0), numByte(0),
    byte(0x00), sample(0x00),
    stage(Stages::STOPPED), playing(false)
{
}

// This one opens a TZX file, loads its contents, tries to put nextBlock
// to the beginning of the first block.
bool TZXFile::load(string const& fileName)
{
    char c;

    // In case of object reuse (really?) don't drop previous data unless the
    // new file actually exists and can be opened.
    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good())
    {
        data.clear();
        while (ifs.get(c))
            data.push_back(c);
    }

    size_t lastDot = fileName.find_last_of('.');
    if (lastDot != string::npos)
    {
        string extension = fileName.substr(lastDot);
        cout << "Extension: " << extension << endl;
        for (size_t i = 0; i < extension.size(); ++i)
        {
            if (extension[i] >= 'A' && extension[i] <= 'Z')
                extension[i] += ('a' - 'A');
        }

        cout << "Extension: " << extension << endl;
        if (extension == ".tap")
        {
            cout << "TAP file." << endl;
            isTap = true;
        }
    }

    // TZX header is:
    // 0-6 - ZXTape!
    // 7   - 0x1A
    // 8   - Major version number (0x01)
    // 9   - Minor version number
    //
    // If the magic checks, store the version numbers.
    if (isTap == false && equal(&magic[0x00], &magic[0x08], data.begin()))
    {
        major = data[0x08];
        minor = data[0x09];
        nextBlock = 0x0A;
        return true;
    }

    nextBlock = 0x00;   // 0x00 means no valid blocks.
    return isTap;
}

void TZXFile::start()
{
    playing = true;
    cout << "Starting tape." << endl;

    if (stage == Stages::STOPPED)
    {
        getNextPulse();
    }
}

void TZXFile::stop()
{
    playing = false;
    cout << "Stopping tape." << endl;
}

bool TZXFile::rewind()
{
    if (nextBlock)
    {
        cout << "Rewinding to the start." << endl;
        nextBlock = isTap ? 0x00 : 0x0A;
        sample = 0x00;
        playing = false;
        return true;
    }

    return false;
}

uint8_t TZXFile::play()
{
    uint8_t s;
    if (playing)
    {
        s = sample;
        getNextSample();
    }
    else
        s = 0;

    return s;
}

bool TZXFile::advance()
{
    if (nextBlock)
    {
        if (isLastBlock() == false)
            pointer = nextBlock;
        return true;
    }

    return false;
}

bool TZXFile::isLastBlock()
{
    return (nextBlock == data.size());
}

bool TZXFile::getBlock()
{
    size_t blockDataLength;

    if (isTap)
    {
        block.type = "Standard Speed Data Block.";
        block.pilotPulseLength = 2168;
        block.syncOnePulseLength = 667;
        block.syncTwoPulseLength = 735;
        block.zeroPulseLength = 855;
        block.onePulseLength = 1710;
        block.pause = 1000;
        blockDataLength = data[pointer + 1] * 0x100 + data[pointer];
        block.data.assign(
                &data[pointer + 2],
                &data[pointer + 2 + blockDataLength]);
        block.pilotLength = (block.data[0] & 0x80) ? 3223 : 8063;

        nextBlock += blockDataLength + 2;   // Tag(1) + Pause(2) + Length(2) + BDL
        stage = Stages::PILOT;
    }
    else
    {
        block.id = data[pointer];
        block.pilotPulseLength = 0;
        block.syncOnePulseLength = 0;
        block.syncTwoPulseLength = 0;
        block.zeroPulseLength = 0;
        block.onePulseLength = 0;
        block.pause = 0;
        blockDataLength = 0;
        block.pilotLength = 0;
        block.usedBitsInLastByte = 8;
        block.data.clear();

        switch (block.id)
        {
            case 0x10:  // Standard Speed Data Block
                block.type = "Standard Speed Data Block.";
                block.pilotPulseLength = 2168;
                block.syncOnePulseLength = 667;
                block.syncTwoPulseLength = 735;
                block.zeroPulseLength = 855;
                block.onePulseLength = 1710;
                block.pause =
                    data[pointer + 2] * 0x100 + data[pointer + 1];
                blockDataLength =
                    data[pointer + 4] * 0x100 + data[pointer + 3];
                block.data.assign(
                        &data[pointer + 5],
                        &data[pointer + 5 + blockDataLength]);
                block.pilotLength = (block.data[0] & 0x80) ? 3223 : 8063;

                nextBlock += blockDataLength + 5;   // Tag(1) + Pause(2) + Length(2) + BDL
                stage = Stages::PILOT;
                break;

            case 0x11:  // Turbo Speed Data Block
                block.type = "Turbo Speed Data Block.";
                block.pilotPulseLength =
                    data[pointer + 2] * 0x100 + data[pointer + 1];
                block.syncOnePulseLength =
                    data[pointer + 4] * 0x100 + data[pointer + 3];
                block.syncTwoPulseLength =
                    data[pointer + 6] * 0x100 + data[pointer + 5];
                block.zeroPulseLength =
                    data[pointer + 8] * 0x100 + data[pointer + 7];
                block.onePulseLength =
                    data[pointer + 10] * 0x100 + data[pointer + 9];
                block.pilotLength =
                    data[pointer + 12] * 0x100 + data[pointer + 11];
                block.usedBitsInLastByte = data[pointer + 13];
                block.pause =
                    data[pointer + 15] * 0x100 + data[pointer + 14];
                blockDataLength =
                    data[pointer + 18] * 0x10000 
                    + data[pointer + 17] * 0x100
                    + data[pointer + 16];
                block.data.assign(
                        &data[pointer + 19],
                        &data[pointer + 19 + blockDataLength]);

                nextBlock += blockDataLength + 19;
                stage = Stages::PILOT;
                break;

            case 0x12:  // Pure Tone Block
                block.type = "Pure Tone Block.";
                block.pilotPulseLength =
                    data[pointer + 2] * 0x100 + data[pointer + 1];
                block.pilotLength =
                    data[pointer + 4] * 0x100 + data[pointer + 3];

                nextBlock += 5;
                stage = Stages::PILOT;
                break;

            case 0x13:  // Pulse Sequence
                block.type = "Pulse Sequence.";
                block.pilotLength = data[pointer + 1];
                blockDataLength = 2 * block.pilotLength;
                block.data.assign(
                        &data[pointer + 2],
                        &data[pointer + 2 + blockDataLength]);
                nextBlock += blockDataLength + 2;
                stage = Stages::PULSES;
                break;

            case 0x14:  // Pure Data Block
                block.type = "Pure Data Block.";
                block.zeroPulseLength = data[pointer + 2] * 0x100 + data[pointer + 1];
                block.onePulseLength = data[pointer + 4] * 0x100 + data[pointer + 3];
                block.usedBitsInLastByte = data[pointer + 5];
                block.pause = data[pointer + 7] * 0x100 + data[pointer + 6];
                blockDataLength = 
                    data[pointer + 10] * 0x10000
                    + data[pointer + 9] * 0x100 
                    + data[pointer + 8];
                block.data.assign(
                        &data[pointer + 11],
                        &data[pointer + 11 + blockDataLength]);

                nextBlock += blockDataLength + 11;
                stage = Stages::DATA;
                break;

            case 0x20:  // Pause/Stop the tape.
                block.type = "Pause/Stop the tape.";
                block.pause = data[pointer + 2] * 0x100 + data[pointer + 1];
                if (block.pause == 0)
                {
                    playing = false;
                    stage = Stages::STOPPED;
                }
                else
                {
                    stage = Stages::PAUSE;
                }
                nextBlock += 3;
                break;

            case 0x21:  // Group start.
                block.type = "Group start.";
                blockDataLength = data[pointer + 1];
                nextBlock += blockDataLength + 2;
                break;

            case 0x22:  // Group end.
                block.type = "Group end.";
                ++nextBlock;
                break;

            case 0x2A:  // Stop the tape if in 48K mode.
                block.type = "Stop the tape if in 48K mode.";
                playing = false;
                stage = Stages::STOPPED;
                nextBlock += 5;
                break;

            case 0x30:  // Text description.
                block.type = "Text description.";
                nextBlock += dumpComment() + 2;
                break;

            case 0x32:  // Archive info.
                block.type = "Archive info.";
                nextBlock += dumpArchiveInfo() + 3;
                stage = Stages::STOPPED;
                break;

            case 0x33:  // Hardware type.
                block.type = "Hardware type.";
                nextBlock += 3 * data[pointer + 1] + 2;
                stage = Stages::STOPPED;
                break;

            case 0x5A:  // Glue Block. (Skip)
                block.type = "Glue block.";
                nextBlock += 10;
                stage = Stages::STOPPED;
                break;

            default:
                break;
        }
    }

    // Update first data byte
    byte = (block.data.empty()) ? 0x00 : block.data[0];
    // Sanity check.
    return (nextBlock <= data.size());
}

void TZXFile::dumpBlockInfo()
{
    cout << "--- Begin block ---" << endl;
    cout << "Block type: " << block.type << endl;
    cout << "Pilot Pulse Length: " << block.pilotPulseLength << endl;
    cout << "First Sync Pulse Length: " << block.syncOnePulseLength << endl;
    cout << "Second Sync Pulse Length: " << block.syncTwoPulseLength << endl;
    cout << "Zero Pulse Length: " << block.zeroPulseLength << endl;
    cout << "One Pulse Length: " << block.onePulseLength << endl;
    cout << "Pilot length: " << block.pilotLength << endl;
    cout << "Used bits in last byte: " << block.usedBitsInLastByte << endl;
    cout << "Pause: " << block.pause << endl;
    cout << "Data Length: " << block.data.size() << endl;
    cout << "--- End block ---" << endl;
}

size_t TZXFile::dumpArchiveInfo()
{
    string tag;
    string text;
    size_t len;

    size_t length = data[pointer + 2] * 0x100 + data[pointer + 1];
    size_t numStrings = data[pointer + 3];
    size_t index = 4;

    cout << "--- Archive info block ---" << endl;
    for (size_t i = 0; i < numStrings; ++i)
    {
        switch (data[pointer + index])
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

        len = data[pointer + index + 1];
        text.clear();
        for (size_t i = 0; i < len; ++i)
            text.push_back(static_cast<char>(data[pointer + index + 2 + i]));

        cout << tag << text << endl;
        index += len + 2;
    }

    cout << pointer + index << endl;
    cout << nextBlock + length << endl;
    return length;
}

size_t TZXFile::dumpComment()
{
    string text;
    size_t length = data[pointer + 1];

    for (size_t i = 0; i < length; ++i)
        text.push_back(static_cast<char>(data[pointer + 2 + i]));
    cout << "Comment: " << text << endl;

    return length;
}

void TZXFile::getNextSample()
{
    --numSamples;
    if (numSamples % pulseLength == 0)
    {
        sample = ~sample;
    }
    
    if (numSamples == 0)
    {
        getNextPulse();
    }
}

void TZXFile::getNextPulse()
{
    bool again = false;

    do
    {
        again = false;
        switch (stage)
        {
            case Stages::PILOT:
                cout << "Pilot." << endl;
                // First pulse is high.
                pulseLength = block.pilotPulseLength;
                numPulses = block.pilotLength;
                // We need to end high, so if we've got an odd number of
                // pulses we start on 0xFF, and if we've got an even number,
                // we start on 0x00.
                sample = (numPulses % 2) ? 0xFF : 0x00;

                if (block.syncOnePulseLength)
                    stage = Stages::SYNC_ONE;
                else
                    stage = Stages::STOPPED;
                break;

            case Stages::SYNC_ONE:
                cout << "Sync 1." << endl;
                pulseLength = block.syncOnePulseLength;
                numPulses = 1;
                sample = 0x00;
                stage = Stages::SYNC_TWO;
                break;

            case Stages::SYNC_TWO:
                cout << "Sync 2." << endl;
                pulseLength = block.syncTwoPulseLength;
                numPulses = 1;
                sample = 0xFF;
                stage = Stages::DATA;
                break;

            case Stages::PULSES:
                cout << "Pulse." << endl;
                if (numByte < block.data.size())
                {
                    numPulses = 1;
                    pulseLength =
                        block.data[numByte + 1] * 0x100 + block.data[numByte];
                    numByte += 2;
                    break;
                }
                stage = Stages::STOPPED;
                again = true;
                break;

            case Stages::DATA:
                pulseLength = (byte & 0x80) ? 
                    block.onePulseLength : block.zeroPulseLength;
                numPulses = 2;

                byte <<= 1;
                numBit = (numBit + 1) % 8;
                // If we've finished with this byte...
                if (numBit == 0)
                {
                    ++numByte;
                    // If this is not the last byte, get the next one.
                    if (numByte < block.data.size())
                        byte = block.data[numByte];
                    // If it is, start the pause.
                    // (again = false, because we need to output the last bit.)
                    else
                        stage = Stages::PAUSE;
                }
                // Another possibility is that we have finished with the *used*
                // bits in the last byte.
                else if ((numByte == (block.data.size() - 1))
                        && (numBit == block.usedBitsInLastByte))
                {
                    stage = Stages::PAUSE;
                }
                break;

            case Stages::PAUSE:
                cout << "Pause." << endl;
                sample = 0x00;
                if (block.pause)
                {
                    pulseLength = 3500 * block.pause; // 1 millisecond = 3500 T-States.
                    numPulses = 1;
                }
                else
                {
                    again = true;
                }
                stage = Stages::STOPPED;
                break;

            case Stages::STOPPED:
                cout << "Stopped." << endl;
                numByte = 0;
                numBit = 0;
                numPulses = 0;
                pulseLength = 0;
                // If there is another block, continue. If not, stop.
                if ((isLastBlock() == false) && (playing == true))
                {
                    pointer = nextBlock;
                    getBlock(); // Here we set the starting stage.
                    dumpBlockInfo();
                    advance();
                    again = true;
                    break;
                }
                playing = false;
                stage = Stages::STOPPED;
                break;

            default:
                playing = false;
                stage = Stages::STOPPED;
                break;
        }
    } while (again);

    numSamples = numPulses * pulseLength;
}

// vim: et:sw=4:ts=4
