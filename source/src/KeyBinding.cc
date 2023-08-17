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

#include "CommonDefs.h"

#include <SFML/Window/Keyboard.hpp>

#include <map>

using namespace std;
using namespace sf;

/**
 * Mapping for single keys. These are the 40 keys of the original
 * rubber-key Spectrum 48K.
 */
map<Keyboard::Scancode, InputMatrixPosition> zxSingleKeys = {
    // First half-row (B, N, M, Symbol Shift, Space)
    {Keyboard::Scan::B, {0, 0x10}},
    {Keyboard::Scan::N, {0, 0x08}},
    {Keyboard::Scan::M, {0, 0x04}},
    {Keyboard::Scan::LControl, {0, 0x02}},
    {Keyboard::Scan::RControl, {0, 0x02}},
    {Keyboard::Scan::Space, {0, 0x01}},

    // Second half-row (H, H, K, L, Enter)
    {Keyboard::Scan::H, {1, 0x10}},
    {Keyboard::Scan::J, {1, 0x08}},
    {Keyboard::Scan::K, {1, 0x04}},
    {Keyboard::Scan::L, {1, 0x02}},
    {Keyboard::Scan::Enter, {1, 0x01}},

    // Third half-row (Y, U, I, O, P)
    {Keyboard::Scan::Y, {2, 0x10}},
    {Keyboard::Scan::U, {2, 0x08}},
    {Keyboard::Scan::I, {2, 0x04}},
    {Keyboard::Scan::O, {2, 0x02}},
    {Keyboard::Scan::P, {2, 0x01}},

    // Fourth half-row (6, 7, 8, 9, 0)
    {Keyboard::Scan::Num6, {3, 0x10}},
    {Keyboard::Scan::Num7, {3, 0x08}},
    {Keyboard::Scan::Num8, {3, 0x04}},
    {Keyboard::Scan::Num9, {3, 0x02}},
    {Keyboard::Scan::Num0, {3, 0x01}},

    // Fifth half-row (5, 4, 3, 2, 1)
    {Keyboard::Scan::Num5, {4, 0x10}},
    {Keyboard::Scan::Num4, {4, 0x08}},
    {Keyboard::Scan::Num3, {4, 0x04}},
    {Keyboard::Scan::Num2, {4, 0x02}},
    {Keyboard::Scan::Num1, {4, 0x01}},

    // Sixth half-row (T, R, E, W, Q)
    {Keyboard::Scan::T, {5, 0x10}},
    {Keyboard::Scan::R, {5, 0x08}},
    {Keyboard::Scan::E, {5, 0x04}},
    {Keyboard::Scan::W, {5, 0x02}},
    {Keyboard::Scan::Q, {5, 0x01}},

    // Seventh half-row (G, F, D, S, A)
    {Keyboard::Scan::G, {6, 0x10}},
    {Keyboard::Scan::F, {6, 0x08}},
    {Keyboard::Scan::D, {6, 0x04}},
    {Keyboard::Scan::S, {6, 0x02}},
    {Keyboard::Scan::A, {6, 0x01}},

    // Eighth half-row (V, C, X, Z, Caps Shift)
    {Keyboard::Scan::V, {7, 0x10}},
    {Keyboard::Scan::C, {7, 0x08}},
    {Keyboard::Scan::X, {7, 0x04}},
    {Keyboard::Scan::Z, {7, 0x02}},
    {Keyboard::Scan::LShift, {7, 0x01}},
    {Keyboard::Scan::RShift, {7, 0x01}}
};

/**
 * Double keys that are modified by Caps Shift.
 */
map<Keyboard::Scancode, InputMatrixPosition> zxCapsKeys = {
    {Keyboard::Scan::Tab, {0, 0x02}},       // Extend Mode: Caps Shift + Symbol Shift
    {Keyboard::Scan::Escape, {0, 0x01}},    // Break: Caps Shift + Space
    {Keyboard::Scan::Delete, {4, 0x01}},    // Edit: Caps Shift + 1
    {Keyboard::Scan::CapsLock, {4, 0x02}},  // Caps Lock: Caps Shift + 2
    {Keyboard::Scan::Home, {4, 0x04}},      // True Video: Caps Shift + 3
    {Keyboard::Scan::End, {4, 0x08}},       // Inv Video: Caps Shift + 4
    {Keyboard::Scan::Left, {4, 0x10}},      // Left Arrow: Caps Shift + 5
    {Keyboard::Scan::Down, {3, 0x10}},      // Down Arrow: Caps Shift + 6
    {Keyboard::Scan::Up, {3, 0x08}},        // Down Arrow: Caps Shift + 7
    {Keyboard::Scan::Right, {3, 0x04}},     // Down Arrow: Caps Shift + 8
    {Keyboard::Scan::Insert, {3, 0x02}},    // Graph Mode: Caps Shift + 9
    {Keyboard::Scan::Backspace, {3, 0x01}}, // Delete: Caps Shift + 0
};

/**
 * Double keys that are modified by Symbol Shift.
 */
map<Keyboard::Scancode, InputMatrixPosition> zxSymbolKeys = {
    {Keyboard::Scan::Comma, {0, 0x08}},     // Comma: Symbol Shift + N
    {Keyboard::Scan::Period, {0, 0x04}},    // Period: Symbol Shift + M
    {Keyboard::Scan::Hyphen, {2, 0x01}}     // Double quote: Symbol Shift + P
};

/**
 * Mapping from joystick data to keys, for
 * Sinclair Port 1/2 and Cursor joystick and extra buttons.
 */
InputMatrixPosition spectrumKeyJoystick[3][10] = {
    {
        {3, 0x10},  // Sinclair Port 1 Left: 6
        {3, 0x08},  // Sinclair Port 1 Right: 7
        {3, 0x04},  // Sinclair Port 1 Down: 8
        {3, 0x02},  // Sinclair Port 1 Up: 9
        {3, 0x01},  // Sinclair Port 1 Fire: 0
        {7, 0x01},  // Pad button 1: Caps Shift
        {7, 0x02},  // Pad button 2: Z
        {7, 0x04},  // Pad button 3: X
        {6, 0x10},  // Pad button 6: G
        {1, 0x10}   // Pad button 7: H
    }, {
        {4, 0x01},  // Sinclair Port 2 Left: 1
        {4, 0x02},  // Sinclair Port 2 Right: 2
        {4, 0x04},  // Sinclair Port 2 Down: 3
        {4, 0x08},  // Sinclair Port 2 Up: 4
        {4, 0x10},  // Sinclair Port 2 Fire: 5
        {0, 0x01},  // Pad button 1: Space
        {0, 0x02},  // Pad button 2: Symbol Shift
        {0, 0x04},  // Pad button 3: M
        {6, 0x10},  // Pad button 6: G
        {1, 0x10}   // Pad button 7: H
    }, {
        {4, 0x10},  // Cursor Left: 5
        {3, 0x04},  // Cursor Right: 8
        {3, 0x10},  // Cursor Down: 6
        {3, 0x08},  // Cursor Up: 7
        {3, 0x01},  // Cursor Fire: 0
        {7, 0x01},  // Pad button 1: Caps Shift
        {7, 0x02},  // Pad button 2: Z
        {7, 0x04},  // Pad button 3: X
        {6, 0x10},  // Pad button 6: G
        {1, 0x10}   // Pad button 7: H
    }
};

/**
 * Amstrad CPC keyboard matrix.
 */
map<Keyboard::Scancode, InputMatrixPosition> cpcKeys = {
    {Keyboard::Scan::NumpadDecimal, {0, 0x80}},
    {Keyboard::Scan::NumpadEnter, {0, 0x40}},
    {Keyboard::Scan::Numpad3, {0, 0x20}},
    {Keyboard::Scan::Numpad6, {0, 0x10}},
    {Keyboard::Scan::Numpad9, {0, 0x08}},
    {Keyboard::Scan::Down, {0, 0x04}},
    {Keyboard::Scan::Right, {0, 0x02}},
    {Keyboard::Scan::Up, {0, 0x01}},

    {Keyboard::Scan::Numpad0, {1, 0x80}},
    {Keyboard::Scan::Numpad2, {1, 0x40}},
    {Keyboard::Scan::Numpad1, {1, 0x20}},
    {Keyboard::Scan::Numpad5, {1, 0x10}},
    {Keyboard::Scan::Numpad8, {1, 0x08}},
    {Keyboard::Scan::Numpad7, {1, 0x04}},
    {Keyboard::Scan::LAlt, {1, 0x02}},
    {Keyboard::Scan::Left, {1, 0x01}},

    {Keyboard::Scan::LControl, {2, 0x80}},
    {Keyboard::Scan::Grave, {2, 0x40}},
    {Keyboard::Scan::LShift, {2, 0x20}},
    {Keyboard::Scan::RShift, {2, 0x20}},
    {Keyboard::Scan::Numpad4, {2, 0x10}},
    {Keyboard::Scan::RBracket, {2, 0x08}},
    {Keyboard::Scan::Enter, {2, 0x04}},
    {Keyboard::Scan::LBracket, {2, 0x02}},
    {Keyboard::Scan::Delete, {2, 0x01}},

    {Keyboard::Scan::Period, {3, 0x80}},
    {Keyboard::Scan::Slash, {3, 0x40}},
    {Keyboard::Scan::Semicolon, {3, 0x20}},
    {Keyboard::Scan::Apostrophe, {3, 0x10}},
    {Keyboard::Scan::P, {3, 0x08}},
    {Keyboard::Scan::Backslash, {3, 0x04}},
    {Keyboard::Scan::Hyphen, {3, 0x02}},
    {Keyboard::Scan::Equal, {3, 0x01}},

    {Keyboard::Scan::Comma, {4, 0x80}},
    {Keyboard::Scan::M, {4, 0x40}},
    {Keyboard::Scan::K, {4, 0x20}},
    {Keyboard::Scan::L, {4, 0x10}},
    {Keyboard::Scan::I, {4, 0x08}},
    {Keyboard::Scan::O, {4, 0x04}},
    {Keyboard::Scan::Num9, {4, 0x02}},
    {Keyboard::Scan::Num0, {4, 0x01}},

    {Keyboard::Scan::Space, {5, 0x80}},
    {Keyboard::Scan::N, {5, 0x40}},
    {Keyboard::Scan::J, {5, 0x20}},
    {Keyboard::Scan::H, {5, 0x10}},
    {Keyboard::Scan::Y, {5, 0x08}},
    {Keyboard::Scan::U, {5, 0x04}},
    {Keyboard::Scan::Num7, {5, 0x02}},
    {Keyboard::Scan::Num8, {5, 0x01}},

    {Keyboard::Scan::V, {6, 0x80}},
    {Keyboard::Scan::B, {6, 0x40}},
    {Keyboard::Scan::F, {6, 0x20}},
    {Keyboard::Scan::G, {6, 0x10}},
    {Keyboard::Scan::T, {6, 0x08}},
    {Keyboard::Scan::R, {6, 0x04}},
    {Keyboard::Scan::Num5, {6, 0x02}},
    {Keyboard::Scan::Num6, {6, 0x01}},

    {Keyboard::Scan::X, {7, 0x80}},
    {Keyboard::Scan::C, {7, 0x40}},
    {Keyboard::Scan::D, {7, 0x20}},
    {Keyboard::Scan::S, {7, 0x10}},
    {Keyboard::Scan::W, {7, 0x08}},
    {Keyboard::Scan::E, {7, 0x04}},
    {Keyboard::Scan::Num3, {7, 0x02}},
    {Keyboard::Scan::Num4, {7, 0x01}},

    {Keyboard::Scan::Z, {8, 0x80}},
    {Keyboard::Scan::CapsLock, {8, 0x40}},
    {Keyboard::Scan::A, {8, 0x20}},
    {Keyboard::Scan::Tab, {8, 0x10}},
    {Keyboard::Scan::Q, {8, 0x08}},
    {Keyboard::Scan::Escape, {8, 0x04}},
    {Keyboard::Scan::Num2, {8, 0x02}},
    {Keyboard::Scan::Num1, {8, 0x01}},

    {Keyboard::Scan::Backspace, {9, 0x80}}
};

InputMatrixPosition cpcJoystick[2][7] = {
    {
        {9, 0x04},  // Joystick 1 Left
        {9, 0x08},  // Joystick 1 Right
        {9, 0x02},  // Joystick 1 Down
        {9, 0x01},  // Joystick 1 Up
        {9, 0x10},  // Joystick 1 Fire 1
        {9, 0x20},  // Joystick 1 Fire 2
        {9, 0x40}   // Joystick 1 Fire 3
    }, {
        {6, 0x04},  // Joystick 2 Left (R)
        {6, 0x08},  // Joystick 2 Right (T)
        {6, 0x02},  // Joystick 2 Down (5)
        {6, 0x01},  // Joystick 2 Up (6)
        {6, 0x10},  // Joystick 2 Fire 1 (G)
        {6, 0x20},  // Joystick 2 Fire 2 (F)
        {6, 0x40}   // Joystick 2 Fire 3 (B)
    }
};
// vim: et:sw=4:ts=4:
