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

/** Z80File.h
 *
 * Z80 file format implementation.
 *
 * This class loads a Z80 snapshot, validates it and applies it to the emulator.
 */

enum class SnapshotModel {
    ZX_48K_ISSUE2,
    ZX_48K_ISSUE3,
    ZX_128K,
    ZX_PLUS2,
    ZX_PLUS2A,
    ZX_PLUS3,
    PENTAGON,
    UNSUPPORTED
};

struct SpectrumStatus {

    uint16_t af = 0x0000;
    uint16_t bc = 0x0000;
    uint16_t de = 0x0000;
    uint16_t hl = 0x0000;

    uint16_t af_ = 0x0000;
    uint16_t bc_ = 0x0000;
    uint16_t de_ = 0x0000;
    uint16_t hl_ = 0x0000;

    uint16_t pc = 0x0000;
    uint16_t sp = 0x0000;
    uint16_t ir = 0x0000;

    uint16_t ix = 0x0000;
    uint16_t iy = 0x0000;

    uint16_t iff = 0x0000;
    uint8_t im = 0x00;

    uint8_t border = 0x00;
    uint8_t joystickType = 0x00;
    bool issue2 = false;

    SnapshotModel model = SnapshotModel::UNSUPPORTED;

    uint8_t port_0x1ffd = 0x00;
    uint8_t port_0x7ffd = 0x00;
    uint8_t port_0xfffd = 0x00;
    uint8_t ayRegs[16];

    bool emuAy8912 = false;
    bool emuFuller = false;

    uint32_t tStates = 0x00;

    bool dataIsCompressed = false;
    uint8_t ramPages[16][0x4000];
};

class Z80File {

    public:
        Z80File() {}

        std::string name;
        uint8_t version = 0;
        uint8_t dataIndex = 0;
        bool valid = false;

        std::vector<uint8_t> fileData;
        SpectrumStatus status;

        void load(std::string const& fileName);
        bool parse();
        void apply();
        void write();

        bool parseStatus();
        bool checkVersion();
        bool fillMemory();
        uint8_t getPage(uint8_t numPage);
        void decompressBlock(size_t length, size_t start, std::vector<uint8_t>& buffer);
};

// vim: et:sw=4:ts=4:
