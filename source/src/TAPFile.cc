#include "TAPFile.h"

void TAPFile::load(string const& fileName)
{
    char c;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good())
    {
        fileData.clear();
        while (ifs.get(c))
            fileData.push_back(c);
    }
}

void TAPFile::parse(
        vector<size_t> &pulseData,
        set<size_t> &indexData,
        set<size_t> &stopData)
{
    static constexpr size_t PILOT_PULSE = 2168;
    static constexpr size_t SYNC_PULSE_1 = 667;
    static constexpr size_t SYNC_PULSE_2 = 735;
    static constexpr size_t DATA_PULSE_0 = 855;
    static constexpr size_t DATA_PULSE_1 = 1710;
    static constexpr size_t PILOT_HEAD_LENGTH = 8063;
    static constexpr size_t PILOT_DATA_LENGTH = 3223;
    static constexpr size_t MILLISECOND_PAUSE = 3500;

    size_t dataLength;
    uint8_t flagByte;
    uint8_t byte;

    // Parse data from the beginning.
    pointer = 0;
    pulseData.clear();
    indexData.clear();
    indexData.insert(0);
    stopData.clear();

    while (pointer < fileData.size())
    {
        // Each block in the ROM loader has:
        // 1. A pilot tone.
        // 2. Two sync pulses.
        // 3. Data.
        // 4. A pause.

        dataLength = fileData[pointer + 1] * 0x100 + fileData[pointer];
        flagByte = fileData[pointer + 2];

        cout << "Block Length: " << dataLength << endl;
        cout << "Flag: " << static_cast<size_t>(flagByte) << endl;

        // Insert the pilot tone.
        pulseData.insert(pulseData.end(),
                (flagByte & 0x80) ? PILOT_DATA_LENGTH : PILOT_HEAD_LENGTH,
                PILOT_PULSE);

        // Insert the sync pulses.
        pulseData.push_back(SYNC_PULSE_1);
        pulseData.push_back(SYNC_PULSE_2);

        // Insert the data.
        for (size_t ii = 0; ii < dataLength; ++ii)
        {
            byte = fileData[pointer + 2 + ii];
            for (size_t jj = 0; jj < 8; ++jj)
            {
                pulseData.insert(pulseData.end(),
                        2, (byte & 0x80) ? DATA_PULSE_1 : DATA_PULSE_0);
                byte <<= 1;
            }
        }

        // Insert the pause. Annotate the pause.
        pulseData.push_back(((flagByte & 0x80) ? 5000 : 1000) * MILLISECOND_PAUSE);
        indexData.insert(pulseData.size());

        pointer += dataLength + 2;
    }

    cout << "Got " << pulseData.size() << " pulses." << endl;
}
