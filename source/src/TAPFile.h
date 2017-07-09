#pragma once

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

/** TAPFile.h
 *
 * TAP file format container.
 *
 * This class generates data suitable for the Tape class.
 */

class TAPFile
{
    public:
    vector<uint8_t> fileData;

    TAPFile() : pointer(0) {}

    void load(string const& fileName);
    void parse(
            vector<size_t> &pulseData,
            set<size_t> &indexData,
            set<size_t> &stopData);

    size_t pointer;
};
// vim: et:sw=4:ts=4:
