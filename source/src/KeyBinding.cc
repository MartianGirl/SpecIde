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

#include "KeyBinding.h"

/**
 * Mapping for single keys. These are the 40 keys of the original
 * rubber-key Spectrum 48K.
 */
KeyBinding singleKeys[42] = {
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
KeyBinding capsKeys[11] = {
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
KeyBinding symbolKeys[3] = {
    {Keyboard::Comma, 0, 0x08},     // Comma: Symbol Shift + N
    {Keyboard::Period, 0, 0x04},    // Period: Symbol Shift + M
    {Keyboard::Quote, 2, 0x01}      // Double quote: Symbol Shift + P
};

/**
 * Mapping from joystick data to keys, for
 * Sinclair Port 1 joystick and extra buttons.
 */
JoystickKeyBinding spectrumKeyJoystick[12] = {

    {3, 0x10},  // Sinclair Port 1 Left: 6
    {3, 0x08},  // Sinclair Port 1 Right: 7
    {3, 0x04},  // Sinclair Port 1 Up: 8
    {3, 0x02},  // Sinclair Port 1 Down: 9
    {3, 0x01},  // Sinclair Port 1 Fire: 0
    {0, 0x10},  // Pad button 1: B
    {0, 0x08},  // Pad button 2: N
    {7, 0x10},  // Pad button 3: V
    {7, 0x08},  // Pad button 4: C
    {7, 0x04},  // Pad button 5: X
    {6, 0x10},  // Pad button 6: G
    {1, 0x10}   // Pad button 7: H
};

/**
 * Amstrad CPC keyboard matrix.
 */
KeyBinding cpcKeys[73] = {
    {Keyboard::Insert, 0, 0x80},
    {Keyboard::RControl, 0, 0x40},
    {Keyboard::Numpad3, 0, 0x20},
    {Keyboard::Numpad6, 0, 0x10},
    {Keyboard::Numpad9, 0, 0x08},
    {Keyboard::Down, 0, 0x04},
    {Keyboard::Right, 0, 0x02},
    {Keyboard::Up, 0, 0x01},

    {Keyboard::Numpad0, 1, 0x80},
    {Keyboard::Numpad2, 1, 0x40},
    {Keyboard::Numpad1, 1, 0x20},
    {Keyboard::Numpad5, 1, 0x10},
    {Keyboard::Numpad8, 1, 0x08},
    {Keyboard::Numpad7, 1, 0x04},
    {Keyboard::LAlt, 1, 0x02},
    {Keyboard::Left, 1, 0x01},

    {Keyboard::LControl, 2, 0x80},
    {Keyboard::BackSlash, 2, 0x40},
    {Keyboard::LShift, 2, 0x20},
    {Keyboard::RShift, 2, 0x20},
    {Keyboard::Numpad4, 2, 0x10},
    {Keyboard::LBracket, 2, 0x08},
    {Keyboard::Enter, 2, 0x04},
    {Keyboard::RBracket, 2, 0x02},
    {Keyboard::Delete, 2, 0x01},

    {Keyboard::Period, 3, 0x80},
    {Keyboard::Slash, 3, 0x40},
    {Keyboard::Semicolon, 3, 0x20},
    {Keyboard::Quote, 3, 0x10},
    {Keyboard::P, 3, 0x08},
    {Keyboard::Add, 3, 0x04},
    {Keyboard::Hyphen, 3, 0x02},
    {Keyboard::Tilde, 3, 0x01},

    {Keyboard::Comma, 4, 0x80},
    {Keyboard::M, 4, 0x40},
    {Keyboard::K, 4, 0x20},
    {Keyboard::L, 4, 0x10},
    {Keyboard::I, 4, 0x08},
    {Keyboard::O, 4, 0x04},
    {Keyboard::Num9, 4, 0x02},
    {Keyboard::Num0, 4, 0x01},

    {Keyboard::Space, 5, 0x80},
    {Keyboard::N, 5, 0x40},
    {Keyboard::J, 5, 0x20},
    {Keyboard::H, 5, 0x10},
    {Keyboard::Y, 5, 0x08},
    {Keyboard::U, 5, 0x04},
    {Keyboard::Num7, 5, 0x02},
    {Keyboard::Num8, 5, 0x01},

    {Keyboard::V, 6, 0x80},
    {Keyboard::B, 6, 0x40},
    {Keyboard::F, 6, 0x20},
    {Keyboard::G, 6, 0x10},
    {Keyboard::T, 6, 0x08},
    {Keyboard::R, 6, 0x04},
    {Keyboard::Num5, 6, 0x02},
    {Keyboard::Num6, 6, 0x01},

    {Keyboard::X, 7, 0x80},
    {Keyboard::C, 7, 0x40},
    {Keyboard::D, 7, 0x20},
    {Keyboard::S, 7, 0x10},
    {Keyboard::W, 7, 0x08},
    {Keyboard::E, 7, 0x04},
    {Keyboard::Num3, 7, 0x02},
    {Keyboard::Num4, 7, 0x01},

    {Keyboard::Z, 8, 0x80},
    {Keyboard::A, 8, 0x20},
    {Keyboard::Tab, 8, 0x10},
    {Keyboard::Q, 8, 0x08},
    {Keyboard::Escape, 8, 0x04},
    {Keyboard::Num2, 8, 0x02},
    {Keyboard::Num1, 8, 0x01},

    {Keyboard::Backspace, 9, 0x80}
};

JoystickKeyBinding cpcJoystick[7] = {

};
// vim: et:sw=4:ts=4:
