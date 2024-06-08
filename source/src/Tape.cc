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

#include "Tape.h"

void Tape::loadTzx(string const& fileName) {

    counter = pulseData.size();

    // Create a .tzx object, load its contents in pulseData.
    TZXFile tzx;
    tzx.load(fileName);
    tzx.parse(pulseData, indexData, stopData, stopIf48K);
    loadData.insert(loadData.end(), tzx.romData.begin(), tzx.romData.end());

    updateFlashTap();
}

void Tape::loadCdt(string const& fileName) {

    counter = pulseData.size();

    // Create a .tzx object, load its contents in pulseData, scaled to
    // match the CPU speed.
    TZXFile cdt;
    cdt.load(fileName);
    cdt.parse(pulseData, indexData, stopData, stopIf48K);
}

void Tape::loadTap(string const& fileName) {

    counter = pulseData.size();

    // Create a .tap object, load its contents in pulseData.
    TAPFile tap;
    tap.load(fileName);
    loadData.insert(loadData.end(), tap.fileData.begin(), tap.fileData.end());
    tap.parse(pulseData, indexData, stopData);

    updateFlashTap();
}

void Tape::loadCsw(string const& fileName) {

    counter = pulseData.size();

    // Create a .csw object, load its contents in pulseData.
    CSWFile csw;
    csw.load(fileName);
    csw.parse(pulseData, indexData, stopData);
}

void Tape::updateFlashTap() {

    cout << "FlashTAP: " << loadData.size() << " bytes." << endl;
    cout << "FlashTAP is load tape." << endl;
    tapData.assign(loadData.begin(), loadData.end());
}

void Tape::play() {

    playing = !playing;
    cout << (playing ? "Playing." : "Stopped.") << endl;
}

void Tape::rewind(size_t position) {

    playing = false;
    sample = 0;
    level = 0x00;
    pointer = position;

    if (position == 0) {
        tapPointer = 0;
    }
    cout << "Rewind to " << position << "..." << endl;
}

void Tape::resetCounter() {

    counter = pointer + 1;
    cout << "Set counter at " << pointer << "..." << endl;
}

void Tape::advance() {

    if (pointer < pulseData.size()) {
        // If we reach an index, we mark it.
        if (indexData.find(pointer) != indexData.end()) {
            cout << "Reached index: " << pointer << endl;
            index = pointer;
        }

        // If we find a stop point, stop and reset level.
        if (stopData.find(pointer) != stopData.end()) {
            cout << "Stopped." << endl;
            playing = false;
        }

        if (is48K && stopIf48K.find(pointer) != stopIf48K.end()) {
            cout << "Stopped in 48K mode." << endl;
            playing = false;
        }

        level ^= 0x7F;
        sample = 2 * pulseData[pointer] * speed;
        ++pointer;
    } else {
        // If we reach the end of the tape, stop, rewind and reset level.
        cout << "End of tape." << endl;
        pointer = 0;
        sample = 0;
        level = 0x00;
        playing = false;
    }
}

void Tape::next() {

    pointer = *(indexData.upper_bound(pointer + 1));
}

void Tape::prev() {

    pointer = *(indexData.lower_bound(pointer - 1));
}

uint_fast8_t Tape::getBlockByte(size_t offset) {

    return tapData[tapPointer + offset];
}

bool Tape::foundTapBlock(uint_fast8_t flag) {

    bool res;
    if (getBlockByte(2) == flag) {
        cout << "Loading - ";
        res = true;
    } else {
        cout << "Found   - ";
        res = false;
    }

    cout << "Flag: " << setw(3) << static_cast<size_t>(getBlockByte(2))
        << "  Length: " << setw(5) << getBlockLength() << endl;
    return res;
}

uint16_t Tape::getBlockLength() {

    return getBlockByte(1) * 0x100 + getBlockByte(0);
}

void Tape::nextTapBlock() {

    tapPointer += 2 + getBlockLength();

    if (tapPointer >= tapData.size()) {
        tapPointer = 0;
    }

    next();
}

void Tape::selectTapData() {

    if (useSaveData) {
        cout << "FlashTAP is save tape." << endl;
        tapData.assign(saveData.begin(), saveData.end());
    } else {
        cout << "FlashTAP is load tape." << endl;
        tapData.assign(loadData.begin(), loadData.end());
    }

    tapPointer = 0;
}

void Tape::writeSaveData() {

    char name[256];
    snprintf(name, 256, "savetape%02u.tap", tapes);
    cout << "Saving to " << name << endl;

    char *data = reinterpret_cast<char*>(&saveData[0]);
    size_t size = saveData.size();
    ofstream ofs(name, std::ofstream::binary);
    ofs.write(data, size);
    ofs.close();

    tapes = (tapes + 1) % 100;
}

void Tape::clearSaveData() {

    cout << "Clearing save buffer." << endl;
    saveData.clear();
}

void Tape::appendLoadData() {

    cout << "Dumping FlashTAP to save buffer." << endl;
    saveData.insert(saveData.end(), tapData.begin(), tapData.end());
}

// vim: et:sw=4:ts=4:
