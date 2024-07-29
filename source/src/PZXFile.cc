/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2024.
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

#include <fstream>
#include <iomanip>
#include <iostream>

#include "PZXFile.h"
#include "Utils.h"

using namespace std;

uint32_t constexpr pzxTagPzxt = 'P' | ('Z' << 8) | ('X' << 16) | ('T' << 24);
uint32_t constexpr pzxTagPuls = 'P' | ('U' << 8) | ('L' << 16) | ('S' << 24);
uint32_t constexpr pzxTagData = 'D' | ('A' << 8) | ('T' << 16) | ('A' << 24);
uint32_t constexpr pzxTagPaus = 'P' | ('A' << 8) | ('U' << 16) | ('S' << 24);
uint32_t constexpr pzxTagBrws = 'B' | ('R' << 8) | ('W' << 16) | ('S' << 24);
uint32_t constexpr pzxTagStop = 'S' | ('T' << 8) | ('O' << 16) | ('P' << 24);

void PZXFile::load(string const& fileName) {

    name = fileName;
    char c;

    ifstream ifs(fileName.c_str(), ifstream::binary);
    if (ifs.good()) {
        fileData.clear();
        while (ifs.get(c)) {
            fileData.push_back(c);
        }
    }
}

void PZXFile::parse(
        vector<uint32_t> &pulseData,
        set<size_t> &indexData,
        set<size_t> &stopData,
        set<size_t> &stopIf48K) {

    uint32_t tag = 0;
    uint32_t len = 0;
    uint32_t nxt = 0;
    uint32_t ptr = 0;

    uint32_t rep = 0;
    uint32_t val = 0;

    uint32_t bits = 0;
    uint16_t tail = 0;
    uint8_t lenPulses[2] {0, 0};
    vector<vector<uint16_t>> seqPulses {vector<uint16_t>(), vector<uint16_t>()};

    bool concatenate = false;
    uint8_t byte = 0;

    string blockName;
    ss.str("");

    ptr = 0;
    romData.clear();

    if (!pulseData.empty()) {
        indexData.insert(pulseData.size());
        stopData.insert(pulseData.size());
    }

    while ((ptr + 8) < fileData.size()) {

        tag = getU32(fileData, ptr); ptr += 4;
        len = getU32(fileData, ptr); ptr += 4;
        nxt = ptr + len;

        if (nxt <= fileData.size()) {
            switch (tag) {
                case pzxTagPzxt:
                    majorVersion = fileData[ptr++];
                    minorVersion = fileData[ptr++];
                    cout << "PZXT ";
                    cout << static_cast<uint32_t>(majorVersion) << ".";
                    cout << static_cast<uint32_t>(minorVersion);
                    break;

                case pzxTagPuls:
                    cout << "PULS ";
                    // This condition should never happen, since PZX authors must ensure
                    // PULS blocks start on LOW level.
                    concatenate = ((pulseData.size() % 2) != 0);
                    while (ptr < nxt) {
                        // Read number of repetitions
                        rep = getU16(fileData, ptr); ptr += 2;
                        if (rep > 0x8000) {
                            rep &= 0x7FFF;
                            if (rep > 1) cout << "[" << rep << " * ";
                            val = getU16(fileData, ptr); ptr += 2;
                        } else {
                            // No repetitions, just pulse length.
                            val = rep;
                            rep = 1;
                        }

                        // Read pulse length
                        if (val & 0x8000) {
                            val = ((val & 0x7FFF) << 16) | getU16(fileData, ptr);
                            ptr += 2;
                        }
                        cout << val;
                        if (rep > 1) cout << "]";
                        cout << " ";

                        // Insert pulses
                        if (val) {
                            if (concatenate) {
                                concatenate = false;
                                pulseData.back() += val;
                                --rep;
                            }
                            if (rep > 0) {
                                pulseData.insert(pulseData.end(), rep, val);
                            }
                        } else if (rep % 2) {   // And val == 0
                            if (!pulseData.size()) {
                                pulseData.push_back(3500);
                                concatenate = false;
                            } else {
                                concatenate = !concatenate;
                            }
                        }
                    }
                    break;

                case pzxTagData:
                    cout << "DATA";
                    bits = getU32(fileData, ptr); ptr += 4;
                    cout << " LEVEL " << ((bits >> 31) & 1);
                    cout << " BYTES " << ((bits & 0x7FFFFFFF) / 8);
                    tail = getU32(fileData, ptr); ptr += 2;
                    cout << " TAIL " << tail;
                    lenPulses[0] = fileData[ptr++];
                    lenPulses[1] = fileData[ptr++];
                    seqPulses[0].clear();
                    seqPulses[1].clear();

                    if (ptr + 2 * lenPulses[0] + 2 * lenPulses[1] < fileData.size()) {
                        for (size_t pp = 0; pp < 2; ++pp) {
                            cout << " BIT" << pp << " " << static_cast<uint32_t>(lenPulses[pp]) << " [";
                            for (size_t bb = 0; bb < lenPulses[pp]; ++bb) {
                                if (bb) cout << " ";
                                seqPulses[pp].push_back(getU16(fileData, ptr)); ptr += 2;
                                cout << seqPulses[pp].back();
                            }
                            cout << "]";
                        }
                    }

                    if (((bits >> 31) & 1) != (pulseData.size() % 2)) {
                        cout << " Polarity mismatch!";
                    }

                    bits &= 0x7FFFFFFF;
                    romData.push_back(((bits >> 3) & 0x00FF));
                    romData.push_back(((bits >> 3) & 0xFF00) >> 8);
                    for (uint32_t ii = 0; ii < bits; ++ii) {
                        if ((ii % 8) == 0) {
                            byte = fileData[ptr++];
                            romData.push_back(byte);
                        } else {
                            byte <<= 1;
                        }

                        size_t bit = ((byte & 0x80) >> 7);
                        pulseData.insert(pulseData.end(), seqPulses[bit].begin(), seqPulses[bit].end());
                    }

                    if (tail) {
                        pulseData.push_back(tail);
                    } else {
                        concatenate = !concatenate;
                    }
                    break;

                case pzxTagPaus:
                    cout << "PAUS";
                    val = getU32(fileData, ptr); ptr += 4;
                    if (((val >> 31) & 1) == (pulseData.size() % 2)) {
                        pulseData.push_back(3500);
                    }
                    pulseData.push_back(val & 0x7FFFFFF);
                    cout << " LEVEL " << ((val >> 31) & 1) << " LENGTH " << (val & 0x7FFFFFFF);
                    break;

                case pzxTagStop:
                    cout << "STOP";
                    val = getU16(fileData, ptr); ptr += 2;
                    if (pulseData.size()) {
                        if (val) {
                            cout << " IF 48K";
                            stopIf48K.insert(pulseData.size());
                        } else {
                            stopData.insert(pulseData.size());
                        }
                    }
                    break;

                case pzxTagBrws:
                    cout << "BRWS";
                    indexData.insert(pulseData.size());
                    break;

                default:
                    cout << static_cast<uint8_t>((tag & 0x000000FF));
                    cout << static_cast<uint8_t>((tag & 0x0000FF00) >> 8);
                    cout << static_cast<uint8_t>((tag & 0x00FF0000) >> 16);
                    cout << static_cast<uint8_t>((tag & 0xFF000000) >> 24);
                    break;
            }
        }

        cout << endl;
        ptr = nxt;
    }

    // Insert a couple of pulses to ensure there is an edge at the end of the tape.
    if (pulseData.size() % 2 == 0) {
        pulseData.push_back(3500);
    }
    pulseData.push_back(3500);
    cout << "Got " << pulseData.size() << " pulses." << endl;
}

// vim: et:sw=4:ts=4
