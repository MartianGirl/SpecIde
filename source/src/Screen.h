/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
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

/** Screen
 *
 * Default Screen implementation.
 *
 * This class emulates a CRT. It draws an image by scanning the pixels
 * in a zig-zag way, just like a TV screen does.
 *
 * At the moment, it also reads the keyboard and processes the window
 * events.
 *
 * Maybe it should be called Console instead of Screen.
 *
 */

#include "Spectrum.h"
#include "Tape.h"

#include "SoundChannel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <queue>
#include <vector>

class Screen
{
    public:
        Screen(size_t scale);
        ~Screen() { window.close(); }

        size_t w;
        size_t h;
        sf::RenderWindow window;
        std::vector<sf::VideoMode> modes;
        sf::VideoMode bestMode;
        size_t suggestedScansSingle;
        size_t suggestedScansDouble;

        Spectrum spectrum;
        Tape tape;
        SoundChannel channel;

        bool playSound = true;
        bool tapeSound = true;
        bool psgSound = true;
        int samples[2];
        size_t skip;
        size_t pulse = 0;
        size_t sample = 0;

        bool done = false;
        bool menu = false;
        bool fullscreen;
        bool doubleScanMode;
        bool smooth;
        bool aychip;
        bool syncToVideo;

        size_t scale;
        size_t xSize, ySize;
        float xOffset, yOffset;
        float xScale, yScale, sScale;

        int delay;

        sf::Texture scrTexture;
        sf::Sprite scrSprite;
        sf::Uint8* pixbuf;

        sf::Font zxFont;

        bool pad;
        bool flashTap;

        uint_fast8_t keyboardMask[8];

        bool cpuInRefresh();
        void checkTapeTraps();
        void writeMemory(uint_fast16_t a, uint_fast8_t d);
        uint_fast8_t readMemory(uint_fast16_t a);
        void trapLdStart();
        void trapSaBytes();

        void run();

        void clock();
        void update();
        void reopenWindow(bool fs);
        void setFullScreen(bool fs);
        void setSmooth(bool sm);
        void set128K(bool is128K);
        void setTapeSound(bool tape);

        void pollEvents();
        void scanKeys(sf::Event const& event);

        void updateMenu();
        
        void texture(size_t x, size_t y);
        void adjust();
};

// vim: et:sw=4:ts=4
