#include "TZXFile.h"

TZXFile::TZXFile() :
    magic { 'Z', 'X', 'T', 'a', 'p', 'e', '!', 0x1A },
    pointer(0), nextBlock(0),
    pulseLength(0), numPulses(0), numSamples(0),
    numBit(0), numByte(0),
    byte(0x00), sample(0x00),
    stage(Stages::STOPPED), playing(false)
{
}

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

    // TZX header is:
    // 0-6 - ZXTape!
    // 7   - 0x1A
    // 8   - Major version number (0x01)
    // 9   - Minor version number
    //
    // If the magic checks, store the version numbers.
    if (equal(&magic[0x00], &magic[0x08], data.begin()))
    {
        major = data[0x08];
        minor = data[0x09];
        pointer = 0x0A;
        nextBlock = 0x0A;
        return true;
    }

    nextBlock = 0x00;   // 0x00 means no valid blocks.
    return false;
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

bool TZXFile::rewind()
{
    if (nextBlock)
    {
        pointer = 0x0A;
        nextBlock = 0x0A;
        sample = 0x00;
        playing = false;
        return true;
    }

    return false;
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

    block.id = data[pointer];
    switch (block.id)
    {
        case 0x10:  // Standard Speed Data Block
            block.pilotPulseLength = 2168;
            block.syncOnePulseLength = 667;
            block.syncTwoPulseLength = 735;
            block.zeroPulseLength = 855;
            block.onePulseLength = 1710;
            block.pause = data[pointer + 2] * 0x100 + data[pointer + 1];
            blockDataLength = data[pointer + 4] * 0x100 + data[pointer + 3];
            block.data.assign(
                    &data[pointer + 5],
                    &data[pointer + 5 + blockDataLength]);
            
            // ROM values
            block.pilotLength = (block.data[0] & 0x80) ? 3223 : 8063;
            block.usedBitsInLastByte = 8;

            nextBlock += blockDataLength + 5;   // Tag(1) + Pause(2) + Length(2) + BDL
            break;

        case 0x11:  // Turbo Speed Data Block
            block.pilotPulseLength = data[pointer + 2] * 0x100 + data[pointer + 1];
            block.syncOnePulseLength = data[pointer + 4] * 0x100 + data[pointer + 3];
            block.syncTwoPulseLength = data[pointer + 6] * 0x100 + data[pointer + 5];
            block.zeroPulseLength = data[pointer + 8] * 0x100 + data[pointer + 7];
            block.onePulseLength = data[pointer + 10] * 0x100 + data[pointer + 9];
            block.pilotLength = data[pointer + 12] * 0x100 + data[pointer + 11];
            block.usedBitsInLastByte = data[pointer + 13];
            block.pause = data[pointer + 15] * 0x100 + data[pointer + 14];
            blockDataLength = data[pointer + 18] * 0x10000 
                + data[pointer + 17] * 0x100 + data[pointer + 16];
            block.data.assign(
                    &data[pointer + 19],
                    &data[pointer + 19 + blockDataLength]);

            nextBlock += blockDataLength + 19;
            break;

        case 0x12:  // Pure Tone Block
            block.pilotPulseLength = data[pointer + 2] * 0x100 + data[pointer + 1];
            block.syncOnePulseLength = 0;
            block.syncTwoPulseLength = 0;
            block.zeroPulseLength = 0;
            block.onePulseLength = 0;
            block.pilotLength = data[pointer + 4] * 0x100 + data[pointer + 3];
            block.usedBitsInLastByte = 0;
            block.pause = 0;
            blockDataLength = 0;
            block.data.clear();

            nextBlock += 5;
            break;

        case 0x14:  // Pure Data Block
            block.pilotPulseLength = 0;
            block.syncOnePulseLength = 0;
            block.syncTwoPulseLength = 0;
            block.zeroPulseLength = data[pointer + 2] * 0x100 + data[pointer + 1];
            block.onePulseLength = data[pointer + 4] * 0x100 + data[pointer + 3];
            block.pilotLength = 0;
            block.usedBitsInLastByte = data[pointer + 5];
            block.pause = data[pointer + 7] * 0x100 + data[pointer + 6];
            blockDataLength = data[pointer + 10] * 0x10000
                + data[pointer + 9] * 0x100 + data[pointer + 8];
            block.data.assign(
                    &data[pointer + 11],
                    &data[pointer + 11 + blockDataLength]);

            nextBlock += blockDataLength + 11;
            break;

        case 0x20:  // Pause / Stop the tape.
            block.pilotPulseLength = 0;
            block.syncOnePulseLength = 0;
            block.syncTwoPulseLength = 0;
            block.zeroPulseLength = 0;
            block.onePulseLength = 0;
            block.pilotLength = 0;
            block.usedBitsInLastByte = 0;
            block.pause = data[pointer + 2] * 0x100 + data[pointer + 1];
            blockDataLength = 0;
            block.data.clear();

            nextBlock += 3;
            break;

        case 0x32:  // Archive info.
            block.pilotPulseLength = 0;
            block.syncOnePulseLength = 0;
            block.syncTwoPulseLength = 0;
            block.zeroPulseLength = 0;
            block.onePulseLength = 0;
            block.pilotLength = 0;
            block.usedBitsInLastByte = 0;
            block.pause = 0;
            blockDataLength = 0;
            block.data.clear();

            nextBlock += (data[pointer + 2] * 0x100) + data[pointer + 1] + 2;
            break;

        case 0x33:  // Hardware info.
            block.pilotPulseLength = 0;
            block.syncOnePulseLength = 0;
            block.syncTwoPulseLength = 0;
            block.zeroPulseLength = 0;
            block.onePulseLength = 0;
            block.pilotLength = 0;
            block.usedBitsInLastByte = 0;
            block.pause = 0;
            blockDataLength = 0;
            block.data.clear();

            nextBlock += 3 * data[pointer + 1] + 2;
            break;

        case 0x5A:  // Glue Block. (Skip)
            nextBlock += 10;
            break;

        default:
            break;
    }

    // Update first data byte
    byte = (block.data.empty()) ? 0x00 : block.data[0];
    // Sanity check.
    return (nextBlock <= data.size());
}

void TZXFile::dumpBlockInfo()
{
    cout << "--- Begin block ---" << endl;
    cout << hex << setw(2) << setfill('0');
    cout << "Block Id: " << block.id << endl;
    cout << dec;
    cout << "Pilot Pulse Length: " << block.pilotPulseLength << endl;
    cout << "First Sync Pulse Length: " << block.syncOnePulseLength << endl;
    cout << "Second Sync Pulse Length: " << block.syncTwoPulseLength << endl;
    cout << "Zero Pulse Length: " << block.zeroPulseLength << endl;
    cout << "One Pulse Length: " << block.onePulseLength << endl;
    cout << "Pilot length: " << block.pilotLength << endl;
    cout << "Used bits in last byte: " << block.usedBitsInLastByte << endl;
    cout << "Pause: " << block.pause << endl;
    cout << "Data Length: " << block.data.size() << endl;
    cout << "Data first two bytes: " << (block.data[1] * 0x100 + block.data[0]) << endl;
    cout << "--- End block ---" << endl;
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
                sample = 0xFF;
                pulseLength = block.pilotPulseLength;
                numPulses = block.pilotLength;
                stage = Stages::SYNC_ONE;
                break;

            case Stages::SYNC_ONE:
                cout << "Sync 1." << endl;
                pulseLength = block.syncOnePulseLength;
                numPulses = 1;
                stage = Stages::SYNC_TWO;
                break;

            case Stages::SYNC_TWO:
                cout << "Sync 2." << endl;
                pulseLength = block.syncTwoPulseLength;
                numPulses = 1;
                stage = Stages::DATA;
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
                    else
                        stage = Stages::PAUSE;
                }
                // Another possibility is that we have finished with the *used*
                // bits in the last byte.
                else if ((numByte == (block.data.size() - 1))
                        && (numBit == block.usedBitsInLastByte))
                    stage = Stages::PAUSE;
                break;

            case Stages::PAUSE:
                numByte = 0;
                numBit = 0;
                sample = 0;
                cout << "Pause." << endl;
                if (block.pause)
                {
                    pulseLength = 3500 * block.pause; // 1 millisecond = 3500 T-States.
                    numPulses = 1;
                }
                else
                    again = true;
                stage = Stages::STOPPED;
                break;

            case Stages::STOPPED:
                cout << "Stopped." << endl;
                numPulses = 0;
                pulseLength = 0;
                // If there is another block, continue. If not, stop.
                if (isLastBlock() == false)
                {
                    stage = Stages::PILOT;
                    getBlock();
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
