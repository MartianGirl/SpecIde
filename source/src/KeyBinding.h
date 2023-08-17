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
#include <map>

#define SJS1 0
#define SJS2 1
#define CURS 2

#define MOVE_L 0
#define MOVE_R 1
#define MOVE_D 2
#define MOVE_U 3
#define FIRE_0 4
#define FIRE_1 5
#define FIRE_2 6
#define FIRE_3 7
#define FIRE_4 8
#define FIRE_5 9
#define K_PAUS 10
#define K_ABRT 11

using namespace sf;

/** ZX Spectrum key binding: Keys that generate a single key press. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxSingleKeys;
/** ZX Spectrum key binding: Keys that generate Caps Shift + keypress. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxCapsKeys;
/** ZX Spectrum key binding: Keys that generate Symbol Shift + keypress. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxSymbolKeys;
/** Amstrad CPC key binding. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> cpcKeys;

/** ZX Spectrum key binding for Sinclair Joysticks. */
extern InputMatrixPosition spectrumKeyJoystick[3][10];
/** Amstrad CPC key binding for joysticks. */
extern InputMatrixPosition cpcJoystick[2][7];
