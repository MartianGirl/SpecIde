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

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <vector>

class Screen : public GraphicWindow
{
    public:
        Screen(size_t scale = 1);

        bool done, reset;
        bool rewind, play;
        bool resetCounter, rewindToCounter;
        bool fullscreen;
        bool smooth;
        bool toggleLoadingSound;

        size_t scale;
        size_t xSize, ySize;
        float xOffset, yOffset;

        sf::Texture scrTexture;
        sf::Sprite scrSprite;
        std::vector<sf::Uint32> pixels;

        bool update();
        void setFullScreen(bool fs);
        void setSmooth(bool sm);
        void setRgbaInput(sf::Uint32* input) { rgbaInput = input; }
        void setVSyncInput(bool* input) { vSyncInput = input; }
        void setHSyncInput(bool* input) { hSyncInput = input; }
        void setHBlankInput(bool* input) { hBlankInput = input; }
        void setVBlankInput(bool* input) { vBlankInput = input; }

        void setKeyboardPort(uint_fast8_t* out) { keyboardDataOut = out; }
        void setJoystickPort(uint_fast8_t* out) { joystickDataOut = out; }

        void pollEvents();
        void scanKeys(sf::Event const& event);

        sf::Uint32 *rgbaInput;
        bool *hSyncInput;
        bool *vSyncInput;
        bool *hBlankInput;
        bool *vBlankInput;

        uint_fast8_t *keyboardDataOut;
        uint_fast8_t keyboardMask[8];

        uint_fast8_t *joystickDataOut;
};

// vim: et:sw=4:ts=4
