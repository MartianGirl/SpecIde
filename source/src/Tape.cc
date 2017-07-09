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
        // Create a .tzx object, load its contents in pulseData.
        TZXFile tzx;
        tzx.load(fileName);
        tzx.parse(pulseData, indexData, stopData);
    }
    else if (extension == ".tap")
    {
        // Create a .tap object, load its contents in pulseData.
        TAPFile tap;
        tap.load(fileName);
        tap.parse(pulseData, indexData, stopData);
    }
}

void Tape::advance()
{
    if (playing)
    {
        if (sample != 0)
        {
            --sample;
        }
        else
        {
            level = ~level;
            
            if (pointer < pulseData.size())
            {
                sample = pulseData[pointer];
                ++pointer;

                // If we find a stop point, stop and reset level.
                if (stopData.find(pointer) != stopData.end())
                {
                    cout << "Stopped." << endl;
                    playing = false;
                    level = 0;
                }

                // If we reach an index, we mark it.
                if (indexData.find(pointer) != indexData.end())
                {
                    cout << "Reached index: " << pointer << endl;
                    index = pointer;
                }
            }
            else
            // If we reach the end of the tape, stop, rewind
            // and reset level.
            {
                cout << "End of tape." << endl;
                pointer = 0;
                sample = 0;
                playing = false;
                level = 0;
            }
        }
    }
}
