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

#pragma once

#include <cstdint>
#include <vector>

#include "SaveState.h"

/** SNAFile.h
 *
 * SNA file format implementation.
 *
 * This class loads a SNA snapshot, validates it and applies it to the emulator.
 */


class SNAFile {

    public:
        SNAFile() {}

        std::string name;
        uint32_t dataIndex = 0;

        std::vector<uint8_t> fileData;
        SaveState state;

        void load(std::string const& fileName);
        bool parse();
        void apply();
        void write();

        bool parseHeader();
        bool fillMemory();
};

// vim: et:sw=4:ts=4:
