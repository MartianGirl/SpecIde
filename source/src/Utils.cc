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

#include <iomanip>
#include <iostream>

#include <zlib.h>

#include "Utils.h"

using namespace std;

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

// vim: et:sw=4:ts=4
