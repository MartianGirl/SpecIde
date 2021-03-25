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

using namespace sf;

struct KeyBinding {

    Keyboard::Key keyName;
    uint8_t row;
    uint8_t key;
};

/**
 * Mapping for single keys. These are the 40 keys of the original
 * rubber-key Spectrum 48K.
 */
KeyBinding singleKeys[] = {
    // First half-row (B, N, M, Symbol Shift, Space)
    {Keyboard::B, 0, 0x10},
    {Keyboard::N, 0, 0x08},
    {Keyboard::M, 0, 0x04},
    {Keyboard::LControl, 0, 0x02},
    {Keyboard::RControl, 0, 0x02},
    {Keyboard::Space, 0, 0x01},

    // Second half-row (H, H, K, L, Enter)
    {Keyboard::H, 1, 0x10},
    {Keyboard::J, 1, 0x08},
    {Keyboard::K, 1, 0x04},
    {Keyboard::L, 1, 0x02},
    {Keyboard::Return, 1, 0x01},

    // Third half-row (Y, U, I, O, P)
    {Keyboard::Y, 2, 0x10},
    {Keyboard::U, 2, 0x08},
    {Keyboard::I, 2, 0x04},
    {Keyboard::O, 2, 0x02},
    {Keyboard::P, 2, 0x01},

    // Fourth half-row (6, 7, 8, 9, 0)
    {Keyboard::Num6, 3, 0x10},
    {Keyboard::Num7, 3, 0x08},
    {Keyboard::Num8, 3, 0x04},
    {Keyboard::Num9, 3, 0x02},
    {Keyboard::Num0, 3, 0x01},

    // Fifth half-row (5, 4, 3, 2, 1)
    {Keyboard::Num5, 4, 0x10},
    {Keyboard::Num4, 4, 0x08},
    {Keyboard::Num3, 4, 0x04},
    {Keyboard::Num2, 4, 0x02},
    {Keyboard::Num1, 4, 0x01},

    // Sixth half-row (T, R, E, W, Q)
    {Keyboard::T, 5, 0x10},
    {Keyboard::R, 5, 0x08},
    {Keyboard::E, 5, 0x04},
    {Keyboard::W, 5, 0x02},
    {Keyboard::Q, 5, 0x01},

    // Seventh half-row (G, F, D, S, A)
    {Keyboard::G, 6, 0x10},
    {Keyboard::F, 6, 0x08},
    {Keyboard::D, 6, 0x04},
    {Keyboard::S, 6, 0x02},
    {Keyboard::A, 6, 0x01},

    // Eighth half-row (V, C, X, Z, Caps Shift)
    {Keyboard::V, 7, 0x10},
    {Keyboard::C, 7, 0x08},
    {Keyboard::X, 7, 0x04},
    {Keyboard::Z, 7, 0x02},
    {Keyboard::LShift, 7, 0x01},
    {Keyboard::RShift, 7, 0x01}
};

/**
 * Double keys that are modified by Caps Shift.
 */
KeyBinding capsKeys[] = {
    {Keyboard::Tab, 0, 0x02},       // Extend Mode: Caps Shift + Symbol Shift
    {Keyboard::Escape, 0, 0x01},    // Break: Caps Shift + Space
    {Keyboard::Delete, 4, 0x01},    // Edit: Caps Shift + 1
    {Keyboard::Home, 4, 0x04},      // True Video: Caps Shift + 3
    {Keyboard::End, 4, 0x08},       // Inv Video: Caps Shift + 4
    {Keyboard::Left, 4, 0x10},      // Left Arrow: Caps Shift + 5
    {Keyboard::Down, 3, 0x10},      // Down Arrow: Caps Shift + 6
    {Keyboard::Up, 3, 0x08},        // Down Arrow: Caps Shift + 7
    {Keyboard::Right, 3, 0x04},     // Down Arrow: Caps Shift + 8
    {Keyboard::Insert, 3, 0x02},    // Graph Mode: Caps Shift + 9
    {Keyboard::Backspace, 3, 0x01}, // Delete: Caps Shift + 0
};

/**
 * Double keys that are modified by Symbol Shift.
 */
KeyBinding symbolKeys[] = {
    {Keyboard::Comma, 0, 0x08},     // Comma: Symbol Shift + N
    {Keyboard::Period, 0, 0x04},    // Period: Symbol Shift + M
    {Keyboard::Quote, 2, 0x01}      // Double quote: Symbol Shift + P
};
