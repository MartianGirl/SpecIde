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

#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <cstdint>

using namespace sf;

struct KeyBinding {

    Keyboard::Key keyName;
    uint8_t row;
    uint8_t key;
};

extern KeyBinding singleKeys[42];
extern KeyBinding capsKeys[11];
extern KeyBinding symbolKeys[3];
extern KeyBinding cpcKeys[73];
