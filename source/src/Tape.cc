#include "Tape.h"

void Tape::load(string const& fileName)
{
    // Parse the file name, find the extension. We'll decide what to do
    // based on this.
    size_t dot = fileName.find_last_of('.');
    string extension;
    if (dot != string::npos)
    {
        // Get the extension in lowercase characters.
        extension = fileName.substr(dot);
        for (size_t ii = 0; ii < extension.size(); ++ii)
        {
            if (extension[ii] >= 'A' && extension[ii] <= 'Z')
                extension[ii] += ('a' - 'A');
        }
    }

    if (extension == ".tzx")
    {
        counter = pulseData.size();

        // Create a .tzx object, load its contents in pulseData.
        TZXFile tzx;
        tzx.load(fileName);
        tzx.parse(pulseData, indexData, stopData, stopIf48K);
        loadData.insert(loadData.end(), tzx.romData.begin(), tzx.romData.end());
    }
    else if (extension == ".tap")
    {
        counter = pulseData.size();

        // Create a .tap object, load its contents in pulseData.
        TAPFile tap;
        tap.load(fileName);
        loadData.insert(loadData.end(), tap.fileData.begin(), tap.fileData.end());
        tap.parse(pulseData, indexData, stopData);
    }

    printf("FlashTAP: %ld bytes.\n", loadData.size());
    printf("FlashTAP is load tape.\n");
    tapData.assign(loadData.begin(), loadData.end());
}

uint_fast8_t Tape::advance()
{
    level ^= 0x7F;

    if (pointer < pulseData.size())
    {
        sample = pulseData[pointer];
        ++pointer;

        // If we reach an index, we mark it.
        if (indexData.find(pointer) != indexData.end())
        {
            printf("Reached index: %ld\n", pointer);
            index = pointer;
        }

        // If we find a stop point, stop and reset level.
        if (stopData.find(pointer) != stopData.end())
        {
            printf("Stopped.\n");
            playing = false;
            level = 0x00;
        }

        if (is48K && stopIf48K.find(pointer) != stopIf48K.end())
        {
            printf("Stopped in 48K mode.\n");
            playing = false;
            level = 0x00;
        }

    }
    else
        // If we reach the end of the tape, stop, rewind
        // and reset level.
    {
        printf("End of tape.\n");
        pointer = 0;
        sample = 0;
        playing = false;
        level = 0x00;
    }

    return level;
}
