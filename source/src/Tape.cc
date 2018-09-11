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

    cout << "FlashTAP: " << loadData.size() << " bytes." << endl;
    cout << "FlashTAP is load tape." << endl;
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
            cout << "Reached index: " << pointer << endl;
            index = pointer;
        }

        // If we find a stop point, stop and reset level.
        if (stopData.find(pointer) != stopData.end())
        {
            cout << "Stopped." << endl;
            playing = false;
            level = 0x00;
        }

        if (is48K && stopIf48K.find(pointer) != stopIf48K.end())
        {
            cout << "Stopped in 48K mode." << endl;
            playing = false;
            level = 0x00;
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
        level = 0x00;
    }

    return level;
}
