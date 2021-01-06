/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2020.
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

#include <algorithm>
#include <iomanip>

#include "FD1793.h"

using namespace std;

void FD1793::clock() {

}

void FD1793::reset() {

}

uint_fast8_t FD1793::read(uint_fast8_t a) {

    static uint_fast8_t retval = 0x00;

    switch (addr) {
        case 0x00:  // State register
            break;
        case 0x01:  // Track register
            break;
        case 0x02:  // Sector register
            break;
        case 0x03:  // Data register
            break;
        case 0x07:  // System register
            break;
    }

    return retval;
}

void FD1793::write(uint_fast8_t value) {

    switch (addr) {
        case 0x00:  // Command register
            break;
        case 0x01:  // Track register
            break;
        case 0x02:  // Sector register
            break;
        case 0x03:  // Data register
            break;
        case 0x07:  // System register
            break;
    }
}

// vim: et:sw=4:ts=4
