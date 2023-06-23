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

using namespace sf;

/** ZX Spectrum key binding: Keys that generate a single key press. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxSingleKeys;
/** ZX Spectrum key binding: Keys that generate Caps Shift + keypress. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxCapsKeys;
/** ZX Spectrum key binding: Keys that generate Symbol Shift + keypress. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> zxSymbolKeys;
/** Amstrad CPC key binding. */
extern std::map<Keyboard::Scancode, InputMatrixPosition> cpcKeys;

/** ZX Spectrum key binding for joystick Sinclair 1. */
extern InputMatrixPosition spectrumKeyJoystick[12];
/** Amstrad CPC key binding for joysticks. */
extern InputMatrixPosition cpcJoystick[2][6];
