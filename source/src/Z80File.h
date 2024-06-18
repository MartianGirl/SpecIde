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

/** Z80File.h
 *
 * Z80 file format implementation.
 *
 * This class loads a Z80 snapshot, validates it and applies it to the emulator.
 */


class Z80File {

    public:
        Z80File() {}

        std::string name;
        uint8_t version = 0;
        uint32_t dataIndex = 0;
        bool valid = false;

        std::vector<uint8_t> fileData;
        SaveState state;

        void load(std::string const& fileName);
        bool parse();
        void apply();
        void write();

        bool parseHeader();
        bool checkVersion();
        bool fillMemory();
        uint8_t getPage(uint8_t numPage);
        void decompressBlock(size_t length, size_t start, std::vector<uint8_t>& buffer);
};

// vim: et:sw=4:ts=4:
