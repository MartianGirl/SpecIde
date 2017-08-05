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
        bool fullscreen;

        size_t scale;
        size_t xSize, ySize;
        float xOffset, yOffset;

        sf::Texture scrTexture;
        sf::Sprite scrSprite;

        bool update();
        void setFullScreen(bool fs);
        void setRgbaInput(sf::Uint32* input) { rgbaInput = input; }
        void setVSyncInput(bool* input) { vSyncInput = input; }
        void setHSyncInput(bool* input) { hSyncInput = input; }
        void setHBlankInput(bool* input) { hBlankInput = input; }
        void setVBlankInput(bool* input) { vBlankInput = input; }

        void setKeyboardPort(uint_fast16_t* in, uint_fast8_t* out)
        {
            keyboardAddressIn = in;
            keyboardDataOut = out;
        }

        void pollEvents();

        sf::Uint32 *rgbaInput;
        bool *hSyncInput;
        bool *vSyncInput;
        bool *hBlankInput;
        bool *vBlankInput;

        uint_fast16_t *keyboardAddressIn;
        uint_fast8_t *keyboardDataOut;
};

// vim: et:sw=4:ts=4
