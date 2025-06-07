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
#include <cmath>

#include <zlib.h>

#include "Utils.h"

using namespace std;

uint8_t averageTable[0x100][0x100];

void printBytes(string const& prefix, size_t len, uint8_t* buf) {

    cout << prefix << " ";
    for (size_t ii = 0; ii < len; ++ii) {
        cout << hex << setw(2) << setfill('0');
        cout << static_cast<uint32_t>(buf[ii]) << " ";
        if ((ii & 0xF) == 0xF) {
        cout << endl << ".." << " ";
        }
    }
    cout << endl;
}

bool inflateBuffer(vector<uint8_t>& in, vector<uint8_t>& out) {

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = in.size();
    infstream.next_in = &in[0];
    infstream.avail_out = out.size();
    infstream.next_out = &out[0];

    inflateInit(&infstream);
    int ret = inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
    out.resize(infstream.total_out);

    return (ret >= 0);
}

uint32_t getU32(vector<uint8_t> const& v, uint_fast32_t i) {
    return (v[i + 3] << 24) | (v[i + 2] << 16) | (v[i + 1] << 8) | v[i];
}

uint32_t getU24(vector<uint8_t> const& v, uint_fast32_t i) {
    return (v[i + 2] << 16) | (v[i + 1] << 8) | v[i];
}

uint16_t getU16(vector<uint8_t> const& v, uint_fast32_t i) {
    return (v[i + 1] << 8) | v[i];
}

/**
 * Fill the table of average colours.
 */
void fillAverageTable() {
    for (size_t i = 0x0; i < 0x100; ++i) {
        for (size_t j = 0x0; j < 0x100; ++j) {
            averageTable[i][j] = static_cast<uint8_t>(sqrt(((i * i) + (j * j)) / 2));
        }
    }
}

/**
 * Average two colours in consecutive positions.
 */
uint32_t average(uint32_t *ptr) {

    uint8_t *pSrc = reinterpret_cast<uint8_t*>(ptr);
#if SPECIDE_BYTE_ORDER == 1
    return (0xFF
            | averageTable[pSrc[1]][pSrc[5]] << 8
            | averageTable[pSrc[2]][pSrc[6]] << 16
            | averageTable[pSrc[3]][pSrc[7]] << 24);
#else
    return (0xFF << 24
            | averageTable[pSrc[2]][pSrc[6]] << 16
            | averageTable[pSrc[1]][pSrc[5]] << 8
            | averageTable[pSrc[0]][pSrc[4]]);
#endif
}

void loadPalette(string const& fileName, vector<uint32_t>& paletteData) {

    paletteData.clear();

    ifstream ifs(fileName.c_str(), ifstream::binary);
    vector<uint8_t> bytes;
    if (ifs.good()) {
        char c;
        while (ifs.get(c)) {
            bytes.push_back(c);
        }
    }

    if (bytes.size() % 4 == 0) {
        for (size_t ii = 0; ii < bytes.size() / 4; ++ii) {
#if SPECIDE_BYTE_ORDER == 1
            uint32_t colour = 0xFF              // Don't care for alpha
                + bytes[4 * ii + 1] * 0x100
                + bytes[4 * ii + 2] * 0x10000
                + bytes[4 * ii + 3] * 0x1000000;
#else
            uint32_t colour = 0xFF000000        // Don't care for alpha
                + bytes[4 * ii + 2] * 0x10000
                + bytes[4 * ii + 1] * 0x100
                + bytes[4 * ii + 0];
#endif
            paletteData.push_back(colour);
        }
    }
}

// vim: et:sw=4:ts=4
