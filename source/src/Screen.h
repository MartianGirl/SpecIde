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
        Screen(uint_fast32_t scale, bool fullscreen);

        Spectrum spectrum;
        Tape tape;
        SoundChannel channel;

        int samples[2];
        uint_fast32_t skip;

        bool done;
        bool fullscreen;
        bool smooth;
        bool squareRootDac;

        uint_fast32_t scale;
        uint_fast32_t xSize, ySize;
        float xOffset, yOffset;

        sf::Texture scrTexture;
        sf::Sprite scrSprite;
        std::vector<sf::Uint32> pixels;

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
        bool update();
        void setFullScreen(bool fs);
        void setSmooth(bool sm);
        void set128K(bool is128K);

        void pollEvents();
        void scanKeys(sf::Event const& event);
        
        uint_fast8_t keyboardMask[8];

        void texture(uint_fast32_t x, uint_fast32_t y);
};

// vim: et:sw=4:ts=4
