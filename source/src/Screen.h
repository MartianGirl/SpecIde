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

#include "GraphicWindow.h"
#include "Spectrum.h"
#include "Tape.h"

#include "SoundChannel.h"

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <queue>
#include <vector>

class Screen : public GraphicWindow
{
    public:
        Screen(size_t scale);

        Spectrum spectrum;
        Tape tape;
        SoundChannel channel;

        int samples[2];
        uint_fast32_t skip;

        bool done = false;
        bool menu = false;
        bool fullscreen;
        bool doubleScanMode;
        bool smooth;
        bool squareRootDac;
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

        uint_fast8_t stereo;

        bool pad;

        bool flashTap;
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

        void pollEvents();
        void scanKeys(sf::Event const& event);

        void updateMenu();
        
        uint_fast8_t keyboardMask[8];

        void texture(size_t x, size_t y);
};

// vim: et:sw=4:ts=4
