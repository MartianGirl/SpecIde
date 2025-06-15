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
#include <string>
#include <vector>

extern uint8_t averageTable[0x100][0x100];

void printBytes(std::string const& prefix, size_t len, uint8_t* buf);

bool inflateBuffer(std::vector<uint8_t>& in, std::vector<uint8_t>& out);

uint32_t getU32(std::vector<uint8_t> const& v, uint_fast32_t i);

uint32_t getU24(std::vector<uint8_t> const& v, uint_fast32_t i);

uint16_t getU16(std::vector<uint8_t> const& v, uint_fast32_t i);

void fillAverageTable();

uint32_t average(uint32_t *ptr);

void loadPalette(std::string const& fileName, std::vector<uint32_t>& paletteData);

/**
 * Return the timer step.
 *
 * This can be used to adjust the sleep delay to a multiple of the quantum, to avoid overshooting.
 */
uint32_t getSleepStepAsMilliseconds();

// vim: et:sw=4:ts=4
